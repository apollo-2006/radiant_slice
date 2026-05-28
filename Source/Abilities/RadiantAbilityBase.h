#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadiantAbilityBase.generated.h"

UCLASS(Abstract)
class RADIANTSLICE_API ARadiantAbilityBase : public AActor
{
    GENERATED_BODY()

public:
    ARadiantAbilityBase();

    // The universal execution command triggered by the player
    virtual void ExecuteAbility();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Ability Economy")
    int32 MaxCharges;

    UPROPERTY(Replicated)
    int32 CurrentCharges;

    UPROPERTY(EditDefaultsOnly, Category = "Ability Economy")
    float CooldownDuration;

    bool bIsOnCooldown;

    // Server RPC to ensure players don't hack their cooldowns
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_ExecuteAbility();

    virtual void PerformAbilityLogic(); // Child classes will override this

    void ResetCooldown();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};