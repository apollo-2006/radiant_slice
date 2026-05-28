#include "RadiantCharacter.h"
#include "TacticalMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Inject our custom TacticalMovementComponent instead of the default one
ARadiantCharacter::ARadiantCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UTacticalMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    // Setup collision capsule for a standard tactical shooter height
    GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);

    // Create a First Person Camera Component
    FPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FPCameraComponent->SetupAttachment(GetCapsuleComponent());
    FPCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f)); // Position camera at eye level
    FPCameraComponent->bUsePawnControlRotation = true; // Let the mouse control the camera
}

void ARadiantCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ARadiantCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARadiantCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Note: In UE5.1+, you'd usually bind Enhanced Input here.
    // We will leave this hook ready for when you set up your Input Actions in the Editor.
}

float ARadiantCharacter::GetVelocityMagnitude() const
{
    return GetVelocity().Size();
}

FVector ARadiantCharacter::GetHeadBoneLocation() const
{
    return GetMesh()->GetSocketLocation(FName("head"));
}