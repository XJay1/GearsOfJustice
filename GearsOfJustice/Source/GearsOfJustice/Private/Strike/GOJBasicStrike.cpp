// Fill out your copyright notice in the Description page of Project Settings.

#include "Strike/GOJBasicStrike.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/GOJBaseCharacter.h"
#include "Components/GOJStaminaComponent.h"
#include "Components/GOJHealthComponent.h"
#include "Components/GOJCombatComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseStrike, All, All);

AGOJBasicStrike::AGOJBasicStrike()
{
    PrimaryActorTick.bCanEverTick = true;
    EndDelegate.BindUObject(this, &AGOJBasicStrike::OnAnimationEnded);
}

void AGOJBasicStrike::BeginPlay()
{
    Super::BeginPlay();
}

void AGOJBasicStrike::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGOJBasicStrike::ExecuteStrike(ACharacter* Character)
{
    CurrentCharacter = Cast<AGOJBaseCharacter>(Character);
    if (!CurrentCharacter || !IsEnoughStamina()) return;

    auto StaminaComponent = GetStaminaComponent();
    if (!StaminaComponent) return;

    StaminaComponent->IncreaseStamina(StrikeInfo.RequiredStamina);
    PlayAttackAnimation(StrikeInfo.StrikeAnimation, CurrentCharacter);
    ApplyDamageToHitCharacters();
}

void AGOJBasicStrike::PlayAttackAnimation(UAnimMontage* Animation, ACharacter* Character)
{
    if (!CanMakeStrike || !Animation) return;

    CanMakeStrike = false;

    const auto BaseCharacter = Cast<AGOJBaseCharacter>(Character);
    if (!BaseCharacter) return;

    const auto CombatComponent = BaseCharacter->FindComponentByClass<UGOJCombatComponent>();

    if (!CombatComponent || !CombatComponent->GetCanMakeHit()) return;

    const USkeletalMeshComponent* Mesh = Character->GetMesh();
    if (!Mesh) return;

    BaseCharacter->LockAllActions();
    CombatComponent->SetCanBlock(false);

    const auto AnimInstance = Mesh->GetAnimInstance();
    if (AnimInstance)
    {
        UE_LOG(LogBaseStrike, Display, TEXT("Playing attack animation: %s"), *Animation->GetName());
        AnimInstance->Montage_Play(Animation);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, Animation);
    }
}

void AGOJBasicStrike::OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogBaseStrike, Display, TEXT("Unlock action"));

    if (CurrentCharacter)
    {
        CurrentCharacter->UnlockAllActions();
        const auto CombatComponent = CurrentCharacter->FindComponentByClass<UGOJCombatComponent>();
        if (CombatComponent)
        {
            CombatComponent->SetCanBlock(true);
        }
        CanMakeStrike = false;
        CurrentCharacter = nullptr;
    }
}

bool AGOJBasicStrike::IsEnoughStamina()
{
    auto StaminaComponent = GetStaminaComponent();
    if (!StaminaComponent) return false;

    return StaminaComponent->GetStamina() >= StrikeInfo.RequiredStamina;
}

UGOJStaminaComponent* AGOJBasicStrike::GetStaminaComponent()
{
    if (!CurrentCharacter) return nullptr;
    return CurrentCharacter->FindComponentByClass<UGOJStaminaComponent>();
}

UGOJCombatComponent* AGOJBasicStrike::GetCombatComponent()
{
    if (!CurrentCharacter) return nullptr;
    return CurrentCharacter->FindComponentByClass<UGOJCombatComponent>();
}

void AGOJBasicStrike::ApplyDamageToHitCharacters()
{
    if (!CurrentCharacter || !GetWorld()) return;

    FVector Start = CurrentCharacter->GetActorLocation();
    FVector End = Start + CurrentCharacter->GetActorForwardVector() * StrikeInfo.HitRange;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(CurrentCharacter);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params))
    {
        UE_LOG(LogBaseStrike, Display, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());

        AActor* HitActor = HitResult.GetActor();
        if (!HitActor) return;

        const auto StaminaComponent = HitActor->FindComponentByClass<UGOJStaminaComponent>();
        if (!StaminaComponent) return;

        const auto CombatComponent = HitActor->FindComponentByClass<UGOJCombatComponent>();
        if (!CombatComponent) return;

        UGOJHealthComponent* HealthComponent = HitActor->FindComponentByClass<UGOJHealthComponent>();
        if (!HealthComponent) return;

        if (!HealthComponent->GetIsDead())
        {
            CombatComponent->IncreaseHitsWithoutDamageCount();
            float DamageCount = CalculateDamage();

            if (CombatComponent && CombatComponent->HitReactionAnimation && !HealthComponent->GetIsDead())
            {
                CombatComponent->PlayHitReaction();
            }

            if (CombatComponent->GetIsBlocking() && StaminaComponent->GetStamina() >= StrikeInfo.Damage)
            {
                float BlockCoast = CalculateBlockCost(DamageCount);
                float Block = CalculateBlock();
                StaminaComponent->IncreaseStamina(BlockCoast);

                DamageCount = DamageCount * (1 - Block);
                HealthComponent->OnTakeAnyDamage(HitActor, DamageCount, nullptr, nullptr, CurrentCharacter);
            }
            else
            {
                CombatComponent->StopBlocking();
                HealthComponent->OnTakeAnyDamage(HitActor, DamageCount, nullptr, nullptr, CurrentCharacter);
            }
        }
        if (HealthComponent->GetIsDead())
        {
            HealthComponent->OnDeath.Broadcast();
        }
    }
}

float AGOJBasicStrike::CalculateBlockCost(float Damage)
{
    float Block = CalculateBlock();

    return (Damage + Block) / 2;
}

float AGOJBasicStrike::CalculateBlock()
{
    const auto StaminaComponent = GetStaminaComponent();
    if (!StaminaComponent) return 0;

    return 0.5 + 0.5 * (StaminaComponent->GetStamina() / StaminaComponent->GetMaxStamina());
}

float AGOJBasicStrike::CalculateDamage()
{
    float Protection = 1.0f;
    const auto CombatComponent = GetCombatComponent();
    if (CombatComponent) Protection = CombatComponent->GetCharacterProtection();

    return StrikeInfo.Damage * Protection * CalculateFatigue() * CalcualateComboMultiple() * CalculateCrit() * CalcuateRandModifier();
}

float AGOJBasicStrike::CalculateFatigue()
{
    const auto StaminaComponent = GetStaminaComponent();
    if (!StaminaComponent) return FatigueBase;
    return FatigueBase + FatigueScale * (StaminaComponent->GetStamina() / StaminaComponent->GetMaxStamina());
}

float AGOJBasicStrike::CalcualateComboMultiple()
{
    const auto CombatComponent = GetCombatComponent();
    if (!CombatComponent) return ComboBase;
    return ComboBase + ComboScale * CombatComponent->GetHitsWithoutDamageCount();
}

float AGOJBasicStrike::CalculateCrit()
{
    float RandomValue = FMath::RandRange(0.0f, 100.0f);
    return (RandomValue < StrikeInfo.CritChance) ? CritMultiplier : 1.0f;
}

float AGOJBasicStrike::CalcuateRandModifier()
{
    return FMath::RandRange(RandModMin, RandModMax);
}