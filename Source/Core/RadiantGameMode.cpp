#include "RadiantGameMode.h"
#include "RadiantGameState.h"
#include "RadiantPlayerController.h"
#include "../Character/RadiantCharacter.h"

ARadiantGameMode::ARadiantGameMode()
{
    // Tell the GameMode to use our custom classes
    GameStateClass = ARadiantGameState::StaticClass();
    PlayerControllerClass = ARadiantPlayerController::StaticClass();
    DefaultPawnClass = ARadiantCharacter::StaticClass();
}

void ARadiantGameMode::BeginPlay()
{
    Super::BeginPlay();
    // Server-side initialization logic goes here
}