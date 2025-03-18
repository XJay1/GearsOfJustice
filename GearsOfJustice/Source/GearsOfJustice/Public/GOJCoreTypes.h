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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float CritChance;

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

// State
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    Exploring UMETA(DisplayName = "Exploring"),
    Combat UMETA(DisplayName = "Combat")
};

// Animation
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);