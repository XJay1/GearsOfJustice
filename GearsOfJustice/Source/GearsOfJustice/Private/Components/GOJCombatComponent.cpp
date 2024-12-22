// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/GOJCombatComponent.h"
#include "Player/GOJBaseCharacter.h"
#include "Components/GOJStaminaComponent.h"
#include "Strike/GOJBasicStrike.h"
#include "Strike/GOJEasyPunch.h"
#include "Strike/GOJStrongPunch.h"
#include "Strike/GOJKick.h"

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
    if (!OwnerCharacter) return;

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
        if (TempStrike)
        {
            AGOJBasicStrike* Strike = GetWorld()->SpawnActor<AGOJBasicStrike>(SelectedStrikeClass);

            if (Strike)
            {
                Strike->ExecuteStrike(OwnerCharacter);
            }
        }
    }
}

void UGOJCombatComponent::PlayHitReaction()
{
    if (!HitReactionAnimation) return;

    const auto Mesh = GetCharacterSkeletalMeshComponent();
    if (!Mesh) return;

    auto AnimInstance = Mesh->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(HitReactionAnimation);
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
    if (!BlockAnimation) return;

    const auto BaseCharacter = GetGOJBaseCharacter();
    if (!BaseCharacter) return;

    BaseCharacter->SetCanWalk(false);

    const auto SkeletalMeshComponent = GetCharacterSkeletalMeshComponent();
    if (!SkeletalMeshComponent) return;

    SetIsBlocking(true);
    SetCanMakeHit(false);

    const auto AnimInstance = SkeletalMeshComponent->GetAnimInstance();
    if (AnimInstance)
    {
        UE_LOG(LogGOJCombatComponent, Display, TEXT("Blocking animation started"));
        AnimInstance->Montage_Play(BlockAnimation, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
    }
}

void UGOJCombatComponent::StopBlocking()
{
    if (!BlockAnimation) return;

    const auto SkeletalMeshComponent = GetCharacterSkeletalMeshComponent();
    if (!SkeletalMeshComponent) return;

    const auto BaseCharacter = GetGOJBaseCharacter();
    if (!BaseCharacter) return;

    SetIsBlocking(false);
    SetCanMakeHit(true);
    BaseCharacter->SetCanWalk(true);

    const auto AnimInstance = SkeletalMeshComponent->GetAnimInstance();
    if (AnimInstance && AnimInstance->Montage_IsPlaying(BlockAnimation))
    {
        UE_LOG(LogGOJCombatComponent, Display, TEXT("Blocking animation stopped"));
        AnimInstance->Montage_Stop(0.2f, BlockAnimation);
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
