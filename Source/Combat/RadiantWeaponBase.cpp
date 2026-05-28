#include "RadiantWeaponBase.h"

ARadiantWeaponBase::ARadiantWeaponBase()
{
    PrimaryActorTick.bCanEverTick = false; // Weapons don't need to tick every frame
    bCanFire = true;
    CurrentAmmo = MaxAmmo;
}

void ARadiantWeaponBase::Fire()
{
    if (!bCanFire || CurrentAmmo <= 0) return;

    CurrentAmmo--;
    bCanFire = false;

    // Start the cooldown timer based on the weapon's fire rate
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ARadiantWeaponBase::ResetFire, FireRate, false);
}

void ARadiantWeaponBase::Reload()
{
    CurrentAmmo = MaxAmmo;
}

void ARadiantWeaponBase::ResetFire()
{
    bCanFire = true;
}