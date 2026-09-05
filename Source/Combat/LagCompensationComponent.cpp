#include "LagCompensationComponent.h"
#include "../Character/RadiantCharacter.h"

ULagCompensationComponent::ULagCompensationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetOwnerRole() == ROLE_Authority)
    {
        SaveFramePackage();
    }
}

void ULagCompensationComponent::SaveFramePackage()
{
    ARadiantCharacter* Character = Cast<ARadiantCharacter>(GetOwner());
    if (!Character) return;

    FFramePackage Package;
    Package.Time = GetWorld()->GetTimeSeconds();
    Package.HeadLocation = Character->GetHeadBoneLocation();

    FrameHistory.AddHead(Package);

    // Trim frames older than the rewind window. The Num() > 1 guard matters:
    // removing the tail when it is also the head would leave GetHead() null and
    // the very next comparison would dereference it.
    while (FrameHistory.Num() > 1)
    {
        float HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
        if (HistoryLength <= MaxRecordTime)
        {
            break;
        }
        FrameHistory.RemoveNode(FrameHistory.GetTail());
    }
}

FFramePackage ULagCompensationComponent::GetHistoricalFrame(float RewindTime)
{
    // A client, or a server on the very first tick, has recorded nothing yet.
    // GetTail() returns null there and the old guard dereferenced it directly.
    if (FrameHistory.Num() == 0) return FFramePackage();

    // Requested time predates anything we still hold: too far behind to rewind to.
    if (FrameHistory.GetTail()->GetValue().Time > RewindTime) return FFramePackage();

    // The list is newest-first (SaveFramePackage uses AddHead), so walking from
    // the head toward the tail walks backwards in time.
    TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Older = FrameHistory.GetHead();
    TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Newer = Older;

    while (Older && Older->GetValue().Time > RewindTime)
    {
        Newer = Older;
        Older = Older->GetNextNode();
    }

    if (!Older) return FFramePackage();

    // RewindTime at or after the newest frame leaves Older == Newer, which made
    // the denominator zero and produced a NaN head location.
    const float FrameSpan = Newer->GetValue().Time - Older->GetValue().Time;
    if (FrameSpan <= KINDA_SMALL_NUMBER)
    {
        return Older->GetValue();
    }

    // Interpolate between the two frames
    const float InterpFraction = FMath::Clamp((RewindTime - Older->GetValue().Time) / FrameSpan, 0.f, 1.f);

    FFramePackage InterpolatedFrame;
    InterpolatedFrame.Time = RewindTime;
    InterpolatedFrame.HeadLocation = FMath::Lerp(Older->GetValue().HeadLocation, Newer->GetValue().HeadLocation, InterpFraction);

    return InterpolatedFrame;
}