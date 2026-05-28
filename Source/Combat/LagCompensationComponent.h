#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

// The data packet saved every tick
USTRUCT()
struct FFramePackage
{
    GENERATED_BODY()

    UPROPERTY()
    float Time;

    UPROPERTY()
    FVector HeadLocation;

    FFramePackage() : Time(0.f), HeadLocation(FVector::ZeroVector) {}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RADIANTSLICE_API ULagCompensationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULagCompensationComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Fetches the rewound hitbox
    FFramePackage GetHistoricalFrame(float RewindTime);

private:
    TDoubleLinkedList<FFramePackage> FrameHistory;
    float MaxRecordTime = 1.0f; // Keep 1 second of history max

    void SaveFramePackage();
};