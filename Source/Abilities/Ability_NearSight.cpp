#include "Ability_NearSight.h"
#include "../Character/RadiantCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

AAbility_NearSight::AAbility_NearSight()
{
    MaxCharges = 1;
    CooldownDuration = 0.0f; // Bought per round, no cooldown
}

void AAbility_NearSight::PerformAbilityLogic()
{
    if (!HasAuthority()) return;

    FVector CameraLoc;
    FRotator CameraRot;
    GetOwner()->GetActorEyesViewPoint(CameraLoc, CameraRot);

    FVector ForwardVector = CameraRot.Vector();
    FVector StartLoc = CameraLoc + (ForwardVector * 100.0f); // Spawn slightly ahead
    FVector EndLoc = StartLoc + (ForwardVector * BlindRadius);

    // Perform a thick sphere sweep to catch enemies in the path
    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    ActorsToIgnore.Add(GetOwner());

    UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(),
        StartLoc,
        EndLoc,
        150.0f, // Radius of the blind projectile
        UEngineTypes::ConvertToTraceType(ECC_Pawn),
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        HitResults,
        true
    );

    for (const FHitResult& Hit : HitResults)
    {
        ARadiantCharacter* Enemy = Cast<ARadiantCharacter>(Hit.GetActor());
        if (Enemy)
        {
            // Trigger the Client RPC on the victim's machine
            Client_ApplyPostProcessBlind(Enemy);
        }
    }
}

void AAbility_NearSight::Client_ApplyPostProcessBlind_Implementation(ARadiantCharacter* TargetCharacter)
{
    if (!TargetCharacter || !TargetCharacter->IsLocallyControlled()) return;

    // This is where you would interface with the camera's PostProcessSettings
    // to dynamically lower the SceneColor tint to black and reduce the ViewDistance.

    // Example hook (assuming a custom function exists on your character):
    // TargetCharacter->ActivateNearsightOverlay(BlindDuration);
}