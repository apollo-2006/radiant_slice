#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RadiantCharacter.generated.h"

UCLASS()
class RADIANTSLICE_API ARadiantCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // IMPORTANT: We must use the ObjectInitializer constructor to swap the movement component
    ARadiantCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FPCameraComponent;

    // Getters for combat math later
    float GetVelocityMagnitude() const;
    FVector GetHeadBoneLocation() const;
};