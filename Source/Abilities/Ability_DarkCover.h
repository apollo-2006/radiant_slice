#pragma once

#include "CoreMinimal.h"
#include "RadiantAbilityBase.h"
#include "Ability_DarkCover.generated.h"

UCLASS()
class RADIANTSLICE_API AAbility_DarkCover : public ARadiantAbilityBase
{
    GENERATED_BODY()

public:
    AAbility_DarkCover();

protected:
    virtual void PerformAbilityLogic() override;

    UPROPERTY(EditDefaultsOnly, Category = "Dark Cover")
    float SmokeRadius = 400.0f; // Standard tactical smoke size

    UPROPERTY(EditDefaultsOnly, Category = "Dark Cover")
    float SmokeDuration = 15.0f;

    // The visual blueprint asset that will be spawned
    UPROPERTY(EditDefaultsOnly, Category = "Dark Cover")
    TSubclassOf<AActor> SmokePayloadClass;
};