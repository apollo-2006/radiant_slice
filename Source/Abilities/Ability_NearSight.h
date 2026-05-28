#pragma once

#include "CoreMinimal.h"
#include "RadiantAbilityBase.h"
#include "Ability_NearSight.generated.h"

UCLASS()
class RADIANTSLICE_API AAbility_NearSight : public ARadiantAbilityBase
{
    GENERATED_BODY()

public:
    AAbility_NearSight();

protected:
    virtual void PerformAbilityLogic() override;

    UPROPERTY(EditDefaultsOnly, Category = "NearSight")
    float BlindRadius = 1500.0f; // Distance the projectile travels

    UPROPERTY(EditDefaultsOnly, Category = "NearSight")
    float BlindDuration = 2.5f;

    // Networked function to tell specific clients to black out their screen
    UFUNCTION(Client, Reliable)
    void Client_ApplyPostProcessBlind(class ARadiantCharacter* TargetCharacter);
};