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
    SetCharacterState(ECharacterState::Exploring);

    Super::BeginPlay();
}

void AGOJBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    FText StatusText = GetDevData(HealthComponent->GetHealth(), StaminaComponent->GetStamina());

    
}

void AGOJBaseCharacter::OnStaminaChanged(float Stamina)
{
    FText StatusText = GetDevData(HealthComponent->GetHealth(), StaminaComponent->GetStamina());

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

    if (CharacterState == ECharacterState::Combat)
    {
        BindCombatAction();
    }
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
    if (!GetCanWalk() || !Controller || AxisValue == 0.0f) return;

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
    if (CombatComponent && CombatComponent->GetCanMakeHit())
    {
        CombatComponent->PerformStrike(EStrikeType::Light);
    }
}

void AGOJBaseCharacter::Kick()
{
    if (CombatComponent && CombatComponent->GetCanMakeHit())
    {
        CombatComponent->PerformStrike(EStrikeType::Kick);
    }
}

void AGOJBaseCharacter::StrongPunch()
{
    if (CombatComponent && CombatComponent->GetCanMakeHit())
    {
        CombatComponent->PerformStrike(EStrikeType::Heavy);
    }
}

void AGOJBaseCharacter::OnStartBlocking()
{
    CombatComponent->StartBlocking();
}

void AGOJBaseCharacter::OnStopBlocking()
{
    UE_LOG(GOJBaseCharacterLog, Display, TEXT("Block button was realesed"));
    CombatComponent->StopBlocking();
}

void AGOJBaseCharacter::OnDeath()
{

    if (UCapsuleComponent* CurrentCapsuleComponent = GetCapsuleComponent())
    {
        CurrentCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (USkeletalMeshComponent* MeshComponent = GetMesh())
    {
        MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
}

void AGOJBaseCharacter::PerformStrikeByStrikeType(EStrikeType StrikeType)
{
    if (!CombatComponent->GetCanMakeHit()) return;

    switch (StrikeType)
    {
        case EStrikeType::Light: EasyPunch(); break;
        case EStrikeType::Heavy: StrongPunch(); break;
        case EStrikeType::Kick: Kick(); break;
        default: break;
    }
}

void AGOJBaseCharacter::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == DieAnimMontage)
    {
        UE_LOG(GOJBaseCharacterLog, Display, TEXT("Death animation ended. Enabling ragdoll."));

        EnableRagdoll();

        SetLifeSpan(1.0f);
    }
}

void AGOJBaseCharacter::EnableRagdoll()
{
    if (!GetMesh()) return;

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

    GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    }

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->StopMovementImmediately();
        Movement->DisableMovement();
    }
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

bool AGOJBaseCharacter::GetIsBlocking() const
{
    if (!CombatComponent) return false;

    return CombatComponent->GetIsBlocking();
}

void AGOJBaseCharacter::LockAllActions()
{
    SetCanWalk(false);
    CombatComponent->SetCanMakeHit(false);
}

void AGOJBaseCharacter::UnlockAllActions()
{
    SetCanWalk(true);
    CombatComponent->SetCanMakeHit(true);
}


void AGOJBaseCharacter::SetCharacterState(ECharacterState NewState) 
{
    if (CharacterState == NewState) return;

    CharacterState = NewState;

    if (CharacterState == ECharacterState::Combat)
    {
        BindCombatAction();
        
    }
    else
    {
        UnbindCombatAction();
    }

    UE_LOG(GOJBaseCharacterLog, Warning, TEXT("Character state changed to: %s"), *UEnum::GetValueAsString(CharacterState));
}

void AGOJBaseCharacter::BindCombatAction()
{
    if (!InputComponent) return;

    InputComponent->BindAction("EasyPunch", IE_Pressed, this, &AGOJBaseCharacter::EasyPunch);
    InputComponent->BindAction("Kick", IE_Pressed, this, &AGOJBaseCharacter::Kick);
    InputComponent->BindAction("StrongPunch", IE_Pressed, this, &AGOJBaseCharacter::StrongPunch);
    InputComponent->BindAction("Block", IE_Pressed, this, &AGOJBaseCharacter::OnStartBlocking);
    InputComponent->BindAction("Block", IE_Released, this, &AGOJBaseCharacter::OnStopBlocking);

    UE_LOG(GOJBaseCharacterLog, Warning, TEXT("Combat actions bound."));
}

void AGOJBaseCharacter::UnbindCombatAction()
{
    if (!InputComponent) return;

    InputComponent->RemoveActionBinding("EasyPunch", IE_Pressed);
    InputComponent->RemoveActionBinding("Kick", IE_Pressed);
    InputComponent->RemoveActionBinding("StrongPunch", IE_Pressed);

    UE_LOG(GOJBaseCharacterLog, Warning, TEXT("Combat actions unbound."));
}