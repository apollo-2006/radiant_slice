#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RadiantGameMode.generated.h"

UCLASS()
class RADIANTSLICE_API ARadiantGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ARadiantGameMode();

    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Match Rules")
    float BuyPhaseDuration = 15.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Match Rules")
    float ActionPhaseDuration = 100.0f;
};