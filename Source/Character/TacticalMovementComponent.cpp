#include "TacticalMovementComponent.h"
#include "GameFramework/Character.h"

UTacticalMovementComponent::UTacticalMovementComponent()
{
    // Tactical Shooter Baselines
    MaxWalkSpeed = 600.0f;
    MaxAcceleration = 4000.0f; // Snappy acceleration
    BrakingFrictionFactor = BaseFriction;
    bUseSeparateBrakingFriction = true;
    AirControl = 0.1f; // Strict air-strafing
}

void UTacticalMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CharacterOwner && CharacterOwner->IsLocallyControlled())
    {
        ProcessTacticalBraking();
    }
}

void UTacticalMovementComponent::ProcessTacticalBraking()
{
    FVector CurrentVelocity = Velocity;
    CurrentVelocity.Z = 0.0f; // Ignore jumping momentum for counter-strafing

    FVector InputVector = GetCurrentAcceleration();
    InputVector.Z = 0.0f;

    if (CurrentVelocity.IsNearlyZero())
    {
        BrakingFrictionFactor = BaseFriction;
        return;
    }

    FVector NormalizedVelocity = CurrentVelocity.GetSafeNormal();
    FVector NormalizedInput = InputVector.GetSafeNormal();

    // Dot product to find angle between current momentum and current key press
    float DotResult = FVector::DotProduct(NormalizedVelocity, NormalizedInput);

    if (InputVector.IsNearlyZero())
    {
        // Hands off keyboard = normal slide to stop
        BrakingFrictionFactor = BaseFriction;
    }
    else if (DotResult < -0.2f)
    {
        // Pushing against momentum = extreme brake
        BrakingFrictionFactor = CounterStrafeFriction;
    }
    else
    {
        // Moving normally
        BrakingFrictionFactor = 0.0f;
    }
}