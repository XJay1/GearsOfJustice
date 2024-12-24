// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/GOJBaseCharacter.h"
#include "GOJCoreTypes.h"
#include "GOJAICharacter.generated.h"

class UBehaviorTree;
class UGOJStaminaComponent;
class UGOJCombatComponent;

UCLASS()
class GEARSOFJUSTICE_API AGOJAICharacter : public AGOJBaseCharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    virtual void OnStartBlocking() override;
    virtual void OnStopBlocking() override;

    void PerformRandomAttack();
    void AttackWithDelay();

    float GetDistanceToPlayer() const;
    float GetSmallestAttackRange() const;
    TArray<EStrikeType> GetAvailableAttack(float DistanceToPlayer) const;
    EStrikeType ChooseRandomAttack(const TArray<EStrikeType>& AvailableAttacks) const;

    EStrikeType GetNextComboAttack();


    FVector GetRandomizedDestination(const FVector& TargetLocation, float OffsetRange) const;

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnAttackAnimationComplete();

    bool IsAttacking() const { return bIsAttacking; }
    void SetIsAttacking(bool InIsAttacking) { bIsAttacking = InIsAttacking; }

    virtual void OnDeath() override;

protected:
    FTimerHandle BlockTimerHandle;
    FTimerDelegate TimerDelegate;

private:
    FTimerHandle AttackTimerHandle;

    bool bIsAttacking = false;

    EStrikeType LastAttackType = EStrikeType::None;

    TArray<EStrikeType> AttackCombo = {EStrikeType::Light, EStrikeType::Heavy, EStrikeType::Kick};
    int32 CurrentComboIndex = 0;
};
