#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RadiantGameState.generated.h"

UCLASS()
class RADIANTSLICE_API ARadiantGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ARadiantGameState();

    // Data that will be replicated to all connected clients
    UPROPERTY(Replicated)
    float CurrentRoundTime;

    UPROPERTY(Replicated)
    bool bIsSpikePlanted;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};