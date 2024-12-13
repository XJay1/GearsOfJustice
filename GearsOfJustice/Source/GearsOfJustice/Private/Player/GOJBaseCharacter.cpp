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
#include "Components/GOJCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Components/GOJStaminaComponent.h"

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

    StaminaComponent = CreateDefaultSubobject<UGOJStaminaComponent>("StaminaComponent");

    CombatComponent = CreateDefaultSubobject<UGOJCombatComponent>("CombatComponent");

    CharacterInfoTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    CharacterInfoTextComponent->SetupAttachment(RootComponent);
    CharacterInfoTextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    CharacterInfoTextComponent->SetTextRenderColor(FColor::Blue);
    CharacterInfoTextComponent->SetWorldSize(50.0f);

}

void AGOJBaseCharacter::BeginPlay()
{
    check(HealthComponent);
    check(StaminaComponent);
    check(CombatComponent);
    check(GetCharacterMovement());
    check(GetMesh());

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    OnStaminaChanged(StaminaComponent->GetStamina());

    HealthComponent->OnHealthChanged.AddUObject(this, &AGOJBaseCharacter::OnHealthChanged);
    HealthComponent->OnDeath.AddUObject(this, &AGOJBaseCharacter::OnDeath);


    StaminaComponent->OnStaminaChangedDelegate.AddUObject(this, &AGOJBaseCharacter::OnStaminaChanged);

    FOnDeadAnimationEnded.BindUObject(this, &AGOJBaseCharacter::OnDeathMontageEnded);

    Super::BeginPlay();
}

void AGOJBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    FText StatusText = GetDevData(HealthComponent->GetHealth(), StaminaComponent->GetStamina());

    CharacterInfoTextComponent->SetText(StatusText);
}

void AGOJBaseCharacter::OnStaminaChanged(float Stamina) 
{
    FText StatusText = GetDevData(HealthComponent->GetHealth(), StaminaComponent->GetStamina());

    CharacterInfoTextComponent->SetText(StatusText);
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
    if (CombatComponent && CanMakeHit)
    {
        CombatComponent->PerformStrike(EStrikeType::Light);
    }
}

void AGOJBaseCharacter::Kick()
{
    if (CombatComponent && CanMakeHit)
    {
        CombatComponent->PerformStrike(EStrikeType::Kick);
    }
}

void AGOJBaseCharacter::StrongPunch()
{
    if (CombatComponent && CanMakeHit)
    {
        CombatComponent->PerformStrike(EStrikeType::Heavy);
    }
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
    // Блокируем движение и управление
    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->DisableMovement();
    }

    if (Controller)
    {
        Controller->SetIgnoreMoveInput(true);
        Controller->SetIgnoreLookInput(true);
    }

    // Проверяем наличие анимации смерти
    auto AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
    if (DieAnimMontage && AnimInstance)
    {
        FOnMontageEnded MontageEndedDelegate;
        // AnimInstance->Montage_SetEndDelegate(EndDelegate, Animation);
        AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, DieAnimMontage);

        AnimInstance->Montage_Play(DieAnimMontage);
    }
    else
    {
        UE_LOG(GOJBaseCharacterLog, Warning, TEXT("No death animation set. Enabling ragdoll immediately."));
        OnDeathMontageEnded(nullptr, true);  // Прямой вызов для включения рэгдолла
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
        UE_LOG(GOJBaseCharacterLog, Display, TEXT("Death animation ended. Enabling ragdoll."));

        // Отключаем анимацию и включаем рэгдолл
         EnableRagdoll();

        // Удаляем персонажа через определенное время (если требуется)
        SetLifeSpan(1.0f);
    }
}

void AGOJBaseCharacter::EnableRagdoll()
{
    if (!GetMesh()) return;

    // Включение физики для Mesh
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

    // Отключение привязки Mesh к Capsule Component
    GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

    // Отключаем Capsule Collision
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    }

    // Отключаем движение
    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->StopMovementImmediately();
        Movement->DisableMovement();
    }

    // Отключаем управление
    if (Controller)
    {
        Controller->SetIgnoreMoveInput(true);
        Controller->SetIgnoreLookInput(true);
    }

    UE_LOG(GOJBaseCharacterLog, Display, TEXT("Ragdoll enabled."));
}


void AGOJBaseCharacter::DestroyCharacter()
{
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(10.0f);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}


FText AGOJBaseCharacter::GetDevData(float Health, float Stamina)
{
    return FText::Format(FText::FromString(TEXT("{0} | {1}")), FText::AsNumber(Health), FText::AsNumber(Stamina));
}

void AGOJBaseCharacter::LockActions()
{
    CanWalk = false;
    CanMakeHit = false;

    GetCharacterMovement()->StopMovementImmediately();
}

void AGOJBaseCharacter::UnlockActions()
{
    CanWalk = true;
    CanMakeHit = true;
}