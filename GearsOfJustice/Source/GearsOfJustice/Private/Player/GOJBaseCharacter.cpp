// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/GOJBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/GOJHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"  
#include "Components/CapsuleComponent.h"               
#include "GameFramework/Character.h"                   

DEFINE_LOG_CATEGORY_STATIC(GOJBaseCharacterLog, All, All);

AGOJBaseCharacter::AGOJBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UGOJHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(RootComponent);
    HealthTextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    HealthTextComponent->SetTextRenderColor(FColor::Blue);
    HealthTextComponent->SetWorldSize(50.0f);

    EndDelegate.BindUObject(this, &AGOJBaseCharacter::OnAnimationEnded);
}

void AGOJBaseCharacter::BeginPlay()
{
    check(HealthComponent);

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    HealthComponent->OnHealthChanged.AddUObject(this, &AGOJBaseCharacter::OnHealthChanged);
    HealthComponent->OnDeath.AddUObject(this, &AGOJBaseCharacter::OnDeath);

    Super::BeginPlay();
}

void AGOJBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    HealthTextComponent->SetText(FText::AsNumber(Health));
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

void AGOJBaseCharacter::OnDeath()
{
    // TODO удалить персонажа

    auto AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
    if (!AnimInstance)
    {
        UE_LOG(GOJBaseCharacterLog, Warning, TEXT("AnimInstance is missing or not set on the Mesh."));
        return;
    }

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->DisableMovement();
    }

    if (Controller)
    {
        Controller->SetIgnoreMoveInput(true);
        Controller->SetIgnoreLookInput(true);
    }

    if (DieAnimMontage)
    {
        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.BindUObject(this, &AGOJBaseCharacter::OnDeathMontageEnded);
        AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, DieAnimMontage);

        AnimInstance->Montage_Play(DieAnimMontage);
    }
    else
    {
        UE_LOG(GOJBaseCharacterLog, Warning, TEXT("DieAnimMontage is not set! Removing character immediately."));
        DestroyCharacter();
    }
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

void AGOJBaseCharacter::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == DieAnimMontage)
    {
        UE_LOG(GOJBaseCharacterLog, Display, TEXT("Death animation ended. Removing character."));
        DestroyCharacter();
    }
}


void AGOJBaseCharacter::DestroyCharacter()
{
    if (Controller)
    {
        Controller->SetIgnoreMoveInput(true);
        Controller->SetIgnoreLookInput(true);

        if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
        {
            PlayerController->UnPossess();
        }
    }
    Destroy();
}
