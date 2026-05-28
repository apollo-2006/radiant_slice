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

    // Prevent memory leaks by trimming old frames
    float HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
    while (HistoryLength > MaxRecordTime)
    {
        FrameHistory.RemoveNode(FrameHistory.GetTail());
        HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
    }
}

FFramePackage ULagCompensationComponent::GetHistoricalFrame(float RewindTime)
{
    if (FrameHistory.GetTail()->GetValue().Time > RewindTime) return FFramePackage();

    TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Older = FrameHistory.GetHead();
    TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Newer = Older;

    while (Older && Older->GetValue().Time > RewindTime)
    {
        Newer = Older;
        Older = Older->GetNextNode();
    }

    if (!Older) return FFramePackage();

    // Interpolate between the two frames
    float InterpFraction = (RewindTime - Older->GetValue().Time) / (Newer->GetValue().Time - Older->GetValue().Time);

    FFramePackage InterpolatedFrame;
    InterpolatedFrame.Time = RewindTime;
    InterpolatedFrame.HeadLocation = FMath::Lerp(Older->GetValue().HeadLocation, Newer->GetValue().HeadLocation, InterpFraction);

    return InterpolatedFrame;
}