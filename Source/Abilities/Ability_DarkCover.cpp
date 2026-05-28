#include "Ability_DarkCover.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AAbility_DarkCover::AAbility_DarkCover()
{
    MaxCharges = 2;
    CooldownDuration = 30.0f; // Signature ability cooldown
}

void AAbility_DarkCover::PerformAbilityLogic()
{
    // Ensure we are on the server
    if (!HasAuthority() || !SmokePayloadClass) return;

    FVector CameraLoc;
    FRotator CameraRot;
    GetOwner()->GetActorEyesViewPoint(CameraLoc, CameraRot);

    // 1. Raycast forward to find where the player is aiming
    FVector TraceEnd = CameraLoc + (CameraRot.Vector() * 5000.0f);

    FHitResult AimHit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(AimHit, CameraLoc, TraceEnd, ECC_Visibility, QueryParams);

    FVector TargetLocation = AimHit.bBlockingHit ? AimHit.ImpactPoint : TraceEnd;

    // 2. Snap the target location to the floor geometry so the smoke doesn't float
    FHitResult FloorHit;
    FVector FloorTraceEnd = TargetLocation - FVector(0, 0, 2000.0f);
    GetWorld()->LineTraceSingleByChannel(FloorHit, TargetLocation, FloorTraceEnd, ECC_WorldStatic, QueryParams);

    FVector SpawnLocation = FloorHit.bBlockingHit ? FloorHit.ImpactPoint : TargetLocation;

    // 3. Spawn the volumetric smoke payload
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = Cast<APawn>(GetOwner());

    AActor* SpawnedSmoke = GetWorld()->SpawnActor<AActor>(SmokePayloadClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

    if (SpawnedSmoke)
    {
        SpawnedSmoke->SetLifeSpan(SmokeDuration);
    }
}