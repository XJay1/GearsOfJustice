// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOJCoreTypes.h"
#include "GOJBasicStrike.generated.h"

class AGOJBaseCharacter;
class UGOJStaminaComponent;
class UGOJCombatComponent;

UCLASS()
class GEARSOFJUSTICE_API AGOJBasicStrike : public AActor
{
    GENERATED_BODY()

public:
    AGOJBasicStrike();

    bool GetCanWalk() const { return CanWalk; }
    bool GetCanMakeStrike() const { return CanMakeStrike; }

    FStrikeInfo GetStrikeInfo() const { return StrikeInfo; }

    void ExecuteStrike(ACharacter* InstigatorCharacter);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Strike")
    FStrikeInfo StrikeInfo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float FatigueBase = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float FatigueScale = 0.5f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float ComboBase = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float ComboScale = 0.05f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float CritMultiplier = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float RandModMin = 0.9f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float RandModMax = 1.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat") 
    float BlockBase = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    float BlockScale = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat") 
    float BlockCostMultiplier = 1.2f;


protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    FOnMontageEnded EndDelegate;
    bool CanWalk = true;
    bool CanMakeStrike = true;

    AGOJBaseCharacter* CurrentCharacter = nullptr;

    void PlayAttackAnimation(UAnimMontage* Animation, ACharacter* InstigatorCharacter);
    void OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted);
    bool IsEnoughStamina();
    UGOJStaminaComponent* GetStaminaComponent();
    UGOJCombatComponent* GetCombatComponent();
    void ApplyDamageToHitCharacters();

    float CalculateBlockCost(float Damage);
    float CalculateBlock();
    float CalculateDamage();
    float CalculateFatigue();
    float CalcualateComboMultiple();
    float CalculateCrit();
    float CalcuateRandModifier();

};
