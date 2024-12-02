#pragma once

#include "GOJCoreTypes.generated.h"

// Health
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

USTRUCT(BlueprintType)
struct FHealth
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;
};