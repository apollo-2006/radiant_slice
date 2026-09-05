#include "HitscanRifle.h"
#include "../Character/RadiantCharacter.h"
#include "LagCompensationComponent.h"
#include "Kismet/GameplayStatics.h"

AHitscanRifle::AHitscanRifle()
{
    CurrentShotIndex = 0;
    InitializeRecoilCurve();
}

void AHitscanRifle::Reload()
{
    Super::Reload();

    // Without this the spray index only ever climbs. Once it passed the end of
    // RecoilCurve the weapon stopped applying recoil for the rest of the match.
    CurrentShotIndex = 0;
}

void AHitscanRifle::InitializeRecoilCurve()
{
    // Hardcoded spray pattern. First 3 pull down, then it sweeps right.
    RecoilCurve.Add(FVector2D(0.0f, -0.5f));
    RecoilCurve.Add(FVector2D(0.0f, -1.2f));
    RecoilCurve.Add(FVector2D(0.0f, -1.8f));
    RecoilCurve.Add(FVector2D(1.5f, -2.0f));
    RecoilCurve.Add(FVector2D(2.5f, -2.1f));
}

void AHitscanRifle::Fire()
{
    if (!bCanFire || CurrentAmmo <= 0) return;

    // Call parent class to handle ammo subtraction and fire rate timers
    Super::Fire();

    ARadiantCharacter* OwnerChar = Cast<ARadiantCharacter>(GetOwner());
    if (!OwnerChar) return;

    // Calculate movement penalty: Slower movement = closer to 1.0 multiplier
    float SpeedRatio = OwnerChar->GetVelocityMagnitude() / 600.0f;
    float AccuracyPenalty = 1.0f + (SpeedRatio * 5.0f); // Up to 500% spread if running

    LocalFire(AccuracyPenalty);
}

void AHitscanRifle::LocalFire(float AccuracyPenalty)
{
    FVector CameraLoc;
    FRotator CameraRot;
    GetOwner()->GetActorEyesViewPoint(CameraLoc, CameraRot);

    // 1. Apply Deterministic Recoil to the Camera
    if (CurrentShotIndex < RecoilCurve.Num())
    {
        CameraRot.Yaw += RecoilCurve[CurrentShotIndex].X;
        CameraRot.Pitch += RecoilCurve[CurrentShotIndex].Y;
        CurrentShotIndex++;
    }

    // 2. Apply RNG Spread scaled by movement speed
    float BaseSpread = 0.5f;
    float FinalSpread = BaseSpread * AccuracyPenalty;

    FVector FireDirection = FMath::VRandCone(CameraRot.Vector(), FMath::DegreesToRadians(FinalSpread));
    FVector TraceEnd = CameraLoc + (FireDirection * 15000.f); // 150 meters

    // 3. Local cosmetic line trace
    FHitResult LocalHit;
    GetWorld()->LineTraceSingleByChannel(LocalHit, CameraLoc, TraceEnd, ECC_Visibility);

    ARadiantCharacter* HitCharacter = Cast<ARadiantCharacter>(LocalHit.GetActor());

    // 4. Send exact firing time to Server
    float TimeOfShot = GetWorld()->GetTimeSeconds();
    Server_ConfirmHit(CameraLoc, TraceEnd, TimeOfShot, HitCharacter);
}

bool AHitscanRifle::Server_ConfirmHit_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, float ClientTimestamp, ARadiantCharacter* Target)
{
    return true; // Anti-cheat checks would go here
}

void AHitscanRifle::Server_ConfirmHit_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, float ClientTimestamp, ARadiantCharacter* Target)
{
    // Authoritative Server Damage Logic
    if (Target)
    {
        // For the vertical slice, we bypass full spatial rollback in this function
        // and apply direct damage. The LagCompComponent handles the math separately.
        UGameplayStatics::ApplyDamage(Target, BaseDamage, nullptr, this, UDamageType::StaticClass());
    }
}