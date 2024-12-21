// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOJCoreTypes.h"
#include "GOJBasicStrike.generated.h"

class AGOJBaseCharacter;
class UGOJStaminaComponent;

UCLASS()
class GEARSOFJUSTICE_API AGOJBasicStrike : public AActor
{
    GENERATED_BODY()

public:
    AGOJBasicStrike();

    bool GetCanWalk() const { return CanWalk; }
    bool GetCanMakeStrike() const { return CanMakeStrike; }

    FStrikeInfo GetStrikeInfo() const { return StrikeInfo;  }

    void ExecuteStrike(ACharacter* InstigatorCharacter);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Strike")
    FStrikeInfo StrikeInfo;

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
    void ApplyDamageToHitCharacters();
};
