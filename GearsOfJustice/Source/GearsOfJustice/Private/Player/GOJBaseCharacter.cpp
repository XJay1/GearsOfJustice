// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/GOJBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AGOJBaseCharacter::AGOJBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void AGOJBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AGOJBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGOJBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AGOJBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AGOJBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("TurnAround", this, &AGOJBaseCharacter::TurnAround);
}

void AGOJBaseCharacter::MoveForward(float AxisValue) 
{
    if (!Controller || AxisValue == 0.0) return;

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, AxisValue);
}

void AGOJBaseCharacter::MoveRight(float AxisValue) 
{
    if (!Controller || AxisValue == 0.0) return;

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, AxisValue);
}


void AGOJBaseCharacter::TurnAround(float AxisValue)
{
    AddControllerYawInput(AxisValue);
}