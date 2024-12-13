// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GOJBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGOJHealthComponent;
class UTextRenderComponent;
class UGOJStaminaComponent;
class UGOJCombatComponent;

UCLASS()
class GEARSOFJUSTICE_API AGOJBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AGOJBaseCharacter();

    void LockActions();
    void UnlockActions();


protected:
    virtual void BeginPlay() override;

    void MoveForward(float AxisValue);

    void MoveRight(float AxisValue);

    void TurnAround(float AxisValue);

    void MoveCharacter(float AxisValue, EAxis::Type Axis);
    void StopBlocking(UAnimMontage* Animation);
    void StartBlocking(UAnimMontage* Animation);

    void EasyPunch();
    void Kick();
    void StrongPunch();

    void OnHealthChanged(float Health, float HealthDelta);
    void OnStaminaChanged(float Stamina);

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
    UTextRenderComponent* CharacterInfoTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UGOJCombatComponent* CombatComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EasyPunchAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* KickAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* StrongPunchAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* BlockAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DieAnimMontage;

    UFUNCTION(BlueprintCallable, Category = "Figting")
    bool GetIsBlocking() const;

public:
    bool IsBlocking = false;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetCanWalk(bool bCanWalk) { CanWalk = bCanWalk; }

    FORCEINLINE bool GetCanWalk() const { return CanWalk; }
    FORCEINLINE bool GetCanStrike() const { return CanMakeHit; }

    void OnStartBlocking();
    void OnStopBlocking();
    void OnDeath();

    void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);
    void DestroyCharacter();

private:
    bool CanMakeHit = true;
    bool CanWalk = true;

    FOnMontageEnded FOnDeadAnimationEnded;

    FText GetDevData(float HelthPercent, float StaminaPercent);
    void EnableRagdoll();
};
