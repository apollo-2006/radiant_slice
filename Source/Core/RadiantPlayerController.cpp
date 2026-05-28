#include "RadiantPlayerController.h"

ARadiantPlayerController::ARadiantPlayerController()
{
    // Constructor logic
}

void ARadiantPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Lock mouse to viewport and hide cursor for FPS gameplay
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    bShowMouseCursor = false;
}