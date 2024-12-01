// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/GOJBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"

DEFINE_LOG_CATEGORY_STATIC(GOJBaseCharacterLog, All, All);

AGOJBaseCharacter::AGOJBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    EndDelegate.BindUObject(this, &AGOJBaseCharacter::OnAnimationEnded);
}

void AGOJBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

bool AGOJBaseCharacter::GetIsBlocking() const
{
    return IsBlocking;
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

    PlayerInputComponent->BindAction("EasyPunch", IE_Pressed, this, &AGOJBaseCharacter::EasyPunch);
    PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &AGOJBaseCharacter::Kick);
    PlayerInputComponent->BindAction("StrongPunch", IE_Pressed, this, &AGOJBaseCharacter::StrongPunch);
    PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AGOJBaseCharacter::OnStartBlocking);
    PlayerInputComponent->BindAction("Block", IE_Released, this, &AGOJBaseCharacter::OnStopBlocking);
}

void AGOJBaseCharacter::MoveForward(float AxisValue)
{
    MoveCharacter(AxisValue, EAxis::X);
}

void AGOJBaseCharacter::MoveRight(float AxisValue)
{
    MoveCharacter(AxisValue, EAxis::Y);
}

void AGOJBaseCharacter::MoveCharacter(float AxisValue, EAxis::Type Axis)
{
    if (!CanWalk || !Controller || AxisValue == 0.0f) return;

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(Axis);
    AddMovementInput(Direction, AxisValue);
}

void AGOJBaseCharacter::TurnAround(float AxisValue)
{
    AddControllerYawInput(AxisValue);
}

void AGOJBaseCharacter::EasyPunch()
{
    PlayAttackAnimation(EasyPunchAnimation);
}

void AGOJBaseCharacter::Kick()
{
    PlayAttackAnimation(KickAnimation);
}


void AGOJBaseCharacter::StrongPunch()
{
    PlayAttackAnimation(StrongPunchAnimation);
}

void AGOJBaseCharacter::PlayAttackAnimation(UAnimMontage* Animation)
{
    if (!CanWalk || !Animation || !GetMesh()) return;

    CanMakeHit = false;
    CanWalk = false;

    auto AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        UE_LOG(GOJBaseCharacterLog, Display, TEXT("Playing attack animation: %s"), *Animation->GetName());
        AnimInstance->Montage_Play(Animation);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, Animation);
    }
}

void AGOJBaseCharacter::OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
    CanMakeHit = true;
    CanWalk = true;
}

void AGOJBaseCharacter::OnStartBlocking()
{
    StartBlocking(BlockAnimMontage);
}


void AGOJBaseCharacter::OnStopBlocking()
{
    StopBlocking(BlockAnimMontage);
}

void AGOJBaseCharacter::StartBlocking(UAnimMontage* Animation)
{
    if (!Animation || !GetMesh()) return;

    IsBlocking = true;
    CanMakeHit = false;
    CanWalk = false;

    auto AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        UE_LOG(GOJBaseCharacterLog, Display, TEXT("Blocking animation started"));
        AnimInstance->Montage_Play(Animation, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
    }
}

void AGOJBaseCharacter::StopBlocking(UAnimMontage* Animation)
{
    if (!Animation || !GetMesh()) return;

    IsBlocking = false;
    CanMakeHit = true;
    CanWalk = true;

    auto AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && AnimInstance->Montage_IsPlaying(Animation))
    {
        UE_LOG(GOJBaseCharacterLog, Display, TEXT("Blocking animation stopped"));
        AnimInstance->Montage_Stop(0.2f, Animation);
    }
}
