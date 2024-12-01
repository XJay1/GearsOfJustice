// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GOJBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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
    void PlayAttackAnimation(UAnimMontage* Animation);
    void StopBlocking(UAnimMontage* Animation);
    void StartBlocking(UAnimMontage* Animation);

    void EasyPunch();
    void Kick();
    void StrongPunch();

    FOnMontageEnded EndDelegate;


    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EasyPunchAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* KickAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* StrongPunchAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* BlockAnimMontage;

    UFUNCTION(BlueprintCallable, Category = "Figting")
    bool GetIsBlocking() const;



public:
    bool CanMakeHit = true;
    bool CanWalk = true;
    bool IsBlocking = false;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    void OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

    void OnStartBlocking();
    void OnStopBlocking();
};
