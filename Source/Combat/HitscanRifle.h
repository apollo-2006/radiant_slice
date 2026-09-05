#pragma once

#include "CoreMinimal.h"
#include "RadiantWeaponBase.h"
#include "HitscanRifle.generated.h"

UCLASS()
class RADIANTSLICE_API AHitscanRifle : public ARadiantWeaponBase
{
    GENERATED_BODY()

public:
    AHitscanRifle();

    virtual void Fire() override;

    // Resets the spray pattern along with the magazine
    virtual void Reload() override;

protected:
    // The Client-Side prediction logic
    void LocalFire(float AccuracyPenalty);

    // The Server RPC to verify the hit
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_ConfirmHit(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, float ClientTimestamp, class ARadiantCharacter* Target);

private:
    // TArray holding the X (Yaw) and Y (Pitch) camera offsets per shot
    TArray<FVector2D> RecoilCurve;
    int32 CurrentShotIndex;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float BaseDamage = 40.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float HeadshotMultiplier = 4.0f;

    void InitializeRecoilCurve();
};