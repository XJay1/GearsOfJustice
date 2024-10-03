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

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
