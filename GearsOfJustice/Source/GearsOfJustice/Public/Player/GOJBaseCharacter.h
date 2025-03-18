// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GOJCoreTypes.h"
#include "GOJBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGOJHealthComponent;
class UGOJStaminaComponent;
class UGOJCombatComponent;

UCLASS()
class GEARSOFJUSTICE_API AGOJBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AGOJBaseCharacter();

protected:
    virtual void BeginPlay() override;

    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);
    void TurnAround(float AxisValue);
    void MoveCharacter(float AxisValue, EAxis::Type Axis);

    void BindCombatAction();
    void UnbindCombatAction();

    

    void PerformStrikeByStrikeType(EStrikeType StrikeType);

    void OnHealthChanged(float Health, float HealthDelta);
    void OnStaminaChanged(float Stamina);

    void EasyPunch();
    void Kick();
    void StrongPunch();

    FOnMontageEnded EndDelegate;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UGOJHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UGOJStaminaComponent* StaminaComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UGOJCombatComponent* CombatComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DieAnimMontage;

public:
    bool IsBlocking = false;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void OnStartBlocking();
    virtual void OnStopBlocking();
    virtual void OnDeath();

    void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);
    void DestroyCharacter();

    bool GetCanWalk() const { return bCanWalk; }
    void SetCanWalk(bool CanWalk) { bCanWalk = CanWalk; }

    UFUNCTION(BlueprintCallable)
    bool GetIsBlocking() const;

    void LockAllActions();
    void UnlockAllActions();
    void SetCharacterState(ECharacterState NewState);

private:
    bool bCanWalk = true;

    FOnMontageEnded FOnDeadAnimationEnded;
    ECharacterState CharacterState = ECharacterState::Exploring;

    FText GetDevData(float HelthPercent, float StaminaPercent);
    void EnableRagdoll();
};
