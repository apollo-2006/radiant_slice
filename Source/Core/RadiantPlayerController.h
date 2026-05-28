#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RadiantPlayerController.generated.h"

UCLASS()
class RADIANTSLICE_API ARadiantPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ARadiantPlayerController();

    virtual void BeginPlay() override;
};