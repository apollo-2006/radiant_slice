#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadiantWeaponBase.generated.h"

UCLASS(Abstract)
class RADIANTSLICE_API ARadiantWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    ARadiantWeaponBase();

    // The primary action bound to Left-Click
    virtual void Fire();
    virtual void Reload();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
    int32 MaxAmmo = 25;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
    int32 CurrentAmmo;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
    float FireRate = 0.1f; // Seconds between shots

    bool bCanFire;
    FTimerHandle FireRateTimerHandle;

    void ResetFire();
};