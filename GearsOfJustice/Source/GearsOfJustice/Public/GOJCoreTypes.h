#pragma once

#include "GOJCoreTypes.generated.h"

// Health
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);


// Stamina
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float);

// Strike

USTRUCT(BlueprintType)
struct FStrikeInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
    float RequiredStamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
    float HitRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
    UAnimMontage* StrikeAnimation;
};

UENUM(BlueprintType)
enum class EStrikeType : uint8
{
    Light UMETA(DisplayName = "Light Stike"),
    Heavy UMETA(DisplayName = "Heavy Stike"),
    Kick UMETA(DisplayName = "Kick Stike"),
    None UMETA(DisplayName = "None")
};

// Animation
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);