#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TacticalMovementComponent.generated.h"

UCLASS()
class RADIANTSLICE_API UTacticalMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    UTacticalMovementComponent();

    // Overriding the core physics tick
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Tactical Physics")
    float BaseFriction = 2.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Tactical Physics")
    float CounterStrafeFriction = 15.0f;

private:
    void ProcessTacticalBraking();
};