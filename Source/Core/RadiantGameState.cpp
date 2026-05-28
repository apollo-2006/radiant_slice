#include "RadiantGameState.h"
#include "Net/UnrealNetwork.h"

ARadiantGameState::ARadiantGameState()
{
    CurrentRoundTime = 0.0f;
    bIsSpikePlanted = false;
}

// This function registers our variables so the server syncs them over the network
void ARadiantGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARadiantGameState, CurrentRoundTime);
    DOREPLIFETIME(ARadiantGameState, bIsSpikePlanted);
}