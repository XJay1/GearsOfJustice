// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/GOJCombatComponent.h"
#include "Player/GOJBaseCharacter.h"
#include "Components/GOJStaminaComponent.h"
#include "Strike/GOJBasicStrike.h"
#include "Strike/GOJEasyPunch.h"
#include "Strike/GOJStrongPunch.h"
#include "Strike/GOJKick.h"
#include "Components/GOJHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGOJCombatComponent, All, All);

UGOJCombatComponent::UGOJCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UGOJCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UGOJCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGOJCombatComponent::PerformStrike(EStrikeType StrikeType)
{
    AGOJBaseCharacter* OwnerCharacter = Cast<AGOJBaseCharacter>(GetOwner());
    if (!OwnerCharacter || !GetCanMakeHit()) return; 

    UClass* SelectedStrikeClass = nullptr;

    switch (StrikeType)
    {
        case EStrikeType::Light: SelectedStrikeClass = LightStrikeClass; break;
        case EStrikeType::Heavy: SelectedStrikeClass = HeavyStrikeClass; break;
        case EStrikeType::Kick: SelectedStrikeClass = KickStrikeClass; break;
    }

    if (!SelectedStrikeClass) return;

    if (auto StaminaComponent = OwnerCharacter->FindComponentByClass<UGOJStaminaComponent>())
    {
        const AGOJBasicStrike* TempStrike = SelectedStrikeClass->GetDefaultObject<AGOJBasicStrike>();
        if (TempStrike && StaminaComponent->HasEnoughStamina(TempStrike->StrikeInfo.RequiredStamina))  
        {
            AGOJBasicStrike* Strike = GetWorld()->SpawnActor<AGOJBasicStrike>(SelectedStrikeClass);

            if (Strike)
            {
                Strike->ExecuteStrike(OwnerCharacter);
                StaminaComponent->IncreaseStamina(TempStrike->StrikeInfo.RequiredStamina);  
            }
        }
    }
}

void UGOJCombatComponent::PlayHitReaction()
{
    if (!HitReactionAnimation || !HitInBlockReactionAnimation || GetIsUnderHit()) return;

    SetIsUnderHit(true);

    const auto BaseCharacter = GetGOJBaseCharacter();
    if (!BaseCharacter) return;

    const auto StaminaComponent = BaseCharacter->FindComponentByClass<UGOJStaminaComponent>();
    if (!StaminaComponent) return;

    const auto Mesh = GetCharacterSkeletalMeshComponent();
    if (!Mesh) return;

    auto AnimInstance = Mesh->GetAnimInstance();
    if (AnimInstance)
    {
        BaseCharacter->SetCanWalk(false);
        SetCanMakeHit(false);
        SetCanBlock(false);
        StaminaComponent->SetStaminaAutoRegen(true);

        UAnimMontage* AnimationToPlay = GetIsBlocking() ? HitInBlockReactionAnimation : HitReactionAnimation;

        AnimInstance->Montage_Play(AnimationToPlay);

        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &UGOJCombatComponent::OnHitMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, AnimationToPlay);
    }
}

FStrikeInfo UGOJCombatComponent::GetStrikeInfo(EStrikeType StrikeType)
{
    UClass* SelectedStrikeClass = nullptr;

    switch (StrikeType)
    {
        case EStrikeType::Light: SelectedStrikeClass = LightStrikeClass; break;

        case EStrikeType::Heavy: SelectedStrikeClass = HeavyStrikeClass; break;

        case EStrikeType::Kick: SelectedStrikeClass = KickStrikeClass; break;

        default: return FStrikeInfo();
    }

    if (SelectedStrikeClass)
    {
        const AGOJBasicStrike* DefaultStrike = SelectedStrikeClass->GetDefaultObject<AGOJBasicStrike>();
        if (DefaultStrike)
        {
            return DefaultStrike->GetStrikeInfo();
        }
    }

    return FStrikeInfo();
}

void UGOJCombatComponent::StartBlocking()
{
    if (!BlockAnimation || !GetCanBlock() || GetIsBlocking()) return;  

    const auto BaseCharacter = GetGOJBaseCharacter();
    if (!BaseCharacter) return;

    BaseCharacter->SetCanWalk(false);

    const auto StaminaComponent = GetGOJStaminaComponent();
    if (!StaminaComponent) return;

    StaminaComponent->SetStaminaAutoRegen(false);

    const auto HealthComponent = GetGOJHealthComponent();
    if (!HealthComponent || HealthComponent->GetIsDead()) return;

    const auto SkeletalMeshComponent = GetCharacterSkeletalMeshComponent();
    if (!SkeletalMeshComponent) return;

    SetIsBlocking(true);
    SetCanMakeHit(false);

    const auto AnimInstance = SkeletalMeshComponent->GetAnimInstance();
    if (AnimInstance)
    {
        if (!AnimInstance->Montage_IsPlaying(BlockAnimation))
        {
            AnimInstance->Montage_Play(BlockAnimation, 1.0f, EMontagePlayReturnType::Duration, 0.0f, true);
            UE_LOG(LogGOJCombatComponent, Display, TEXT("Blocking animation started"));
        }
    }
}

void UGOJCombatComponent::StopBlocking()
{
    if (!BlockAnimation || !GetIsBlocking()) return;

    const auto SkeletalMeshComponent = GetCharacterSkeletalMeshComponent();
    if (!SkeletalMeshComponent) return;

    const auto BaseCharacter = GetGOJBaseCharacter();
    if (!BaseCharacter) return;

    const auto StaminaComponent = GetGOJStaminaComponent();
    if (!StaminaComponent) return;

    StaminaComponent->SetStaminaAutoRegen(true);

    UE_LOG(LogGOJCombatComponent, Display, TEXT("Stamina auto regen: %s"),
        StaminaComponent->GetAutoStaminaRegen() ? TEXT("True") : TEXT("False"));

    SetIsBlocking(false);
    SetCanMakeHit(true);
    BaseCharacter->SetCanWalk(true);

    const auto AnimInstance = SkeletalMeshComponent->GetAnimInstance();
    if (AnimInstance && AnimInstance->Montage_IsPlaying(BlockAnimation))
    {
        AnimInstance->Montage_Stop(0.2f, BlockAnimation);
        UE_LOG(LogGOJCombatComponent, Display, TEXT("Blocking animation stopped"));
    }
}

USkeletalMeshComponent* UGOJCombatComponent::GetCharacterSkeletalMeshComponent()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return nullptr;

    return OwnerCharacter->GetMesh();
}

AGOJBaseCharacter* UGOJCombatComponent::GetGOJBaseCharacter()
{
    const auto Character = GetOwner();
    if (!Character) return nullptr;

    return Cast<AGOJBaseCharacter>(Character);
}

void UGOJCombatComponent::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!Montage) return;

    const auto BaseCharacter = GetGOJBaseCharacter();
    if (!BaseCharacter) return;

    const auto CombatComponent = BaseCharacter->FindComponentByClass<UGOJCombatComponent>();
    if (!CombatComponent) return;

    const auto StaminaComponent = BaseCharacter->FindComponentByClass<UGOJStaminaComponent>();
    if (!StaminaComponent) return;

    SetIsUnderHit(false); 

    StaminaComponent->SetStaminaAutoRegen(true);

    if (Montage == HitInBlockReactionAnimation)
    {
        CombatComponent->SetCanBlock(true);
        SetIsBlocking(false);
        CombatComponent->StartBlocking();  
    }
    else if (Montage == HitReactionAnimation)
    {
        BaseCharacter->SetCanWalk(true);
        CombatComponent->SetCanMakeHit(true);
        CombatComponent->SetCanBlock(true);
    }
}

UGOJStaminaComponent* UGOJCombatComponent::GetGOJStaminaComponent()
{
    const auto Character = GetOwner();
    if (!Character) return nullptr;

    const auto BaseCharacter = Cast<AGOJBaseCharacter>(Character);
    if (!BaseCharacter) return nullptr;

    return BaseCharacter->FindComponentByClass<UGOJStaminaComponent>();
}

UGOJHealthComponent* UGOJCombatComponent::GetGOJHealthComponent()
{
    const auto Character = GetOwner();
    if (!Character) return nullptr;

    const auto BaseCharacter = Cast<AGOJBaseCharacter>(Character);
    if (!BaseCharacter) return nullptr;

    return BaseCharacter->FindComponentByClass<UGOJHealthComponent>();
}