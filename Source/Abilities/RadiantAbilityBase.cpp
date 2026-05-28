#include "RadiantAbilityBase.h"
#include "Net/UnrealNetwork.h"

ARadiantAbilityBase::ARadiantAbilityBase()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true; // Crucial for multiplayer abilities

    MaxCharges = 2;
    CurrentCharges = MaxCharges;
    bIsOnCooldown = false;
}

void ARadiantAbilityBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARadiantAbilityBase, CurrentCharges);
}

void ARadiantAbilityBase::ExecuteAbility()
{
    if (CurrentCharges > 0 && !bIsOnCooldown)
    {
        // Tell the server we are using an ability
        Server_ExecuteAbility();
    }
}

bool ARadiantAbilityBase::Server_ExecuteAbility_Validate()
{
    return (CurrentCharges > 0 && !bIsOnCooldown);
}

void ARadiantAbilityBase::Server_ExecuteAbility_Implementation()
{
    CurrentCharges--;
    bIsOnCooldown = true;

    // Trigger the specific child class logic
    PerformAbilityLogic();

    // Start server-authoritative cooldown timer
    FTimerHandle CooldownTimer;
    GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ARadiantAbilityBase::ResetCooldown, CooldownDuration, false);
}

void ARadiantAbilityBase::ResetCooldown()
{
    bIsOnCooldown = false;
}