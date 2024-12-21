// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GOJAICharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GOJCoreTypes.h"
#include "Components/GOJCombatComponent.h"
#include "Components/GOJStaminaComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(GOJAICharacterLog, All, All);

void AGOJAICharacter::OnStartBlocking()
{
    float BlockDuration = FMath::RandRange(1.0f, 3.0f);

    Super::OnStartBlocking();

    UE_LOG(GOJAICharacterLog, Display, TEXT("AI Character started blocking for %.2f seconds"), BlockDuration);

    TimerDelegate.BindLambda([this]() { OnStopBlocking(); });

    GetWorldTimerManager().SetTimer(BlockTimerHandle, TimerDelegate, BlockDuration, false);
}

void AGOJAICharacter::OnStopBlocking()
{
    Super::OnStopBlocking();
    UE_LOG(GOJAICharacterLog, Display, TEXT("AI Character stopped blocking"));
}

void AGOJAICharacter::AttackWithDelay()
{
    if (!GetWorld()) return;

    float RandomDelay = FMath::RandRange(1.0f, 3.0f);
    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AGOJAICharacter::PerformRandomAttack, RandomDelay, false);
}

void AGOJAICharacter::PerformRandomAttack()
{

    float DistanceToPlayer = GetDistanceToPlayer();
    TArray<EStrikeType> AvailableAttacks = GetAvailableAttack(DistanceToPlayer);

    if (AvailableAttacks.Num() > 0)
    {
        EStrikeType SelectedAttack = ChooseRandomAttack(AvailableAttacks);

        PerformStrikeByStrikeType(SelectedAttack);
        LastAttackType = SelectedAttack;
    }
    else
    {
        UE_LOG(GOJAICharacterLog, Warning, TEXT("No attacks available within range"));
    }
}

EStrikeType AGOJAICharacter::GetNextComboAttack()
{
    EStrikeType NextAttack = AttackCombo[CurrentComboIndex];
    CurrentComboIndex = (CurrentComboIndex + 1) % AttackCombo.Num();
    return NextAttack;
}

void AGOJAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetIsBlocking())
    {
        AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerActor)
        {

            FVector DirectionToPlayer = PlayerActor->GetActorLocation() - GetActorLocation();
            DirectionToPlayer.Z = 0.0f;

            FRotator TargetRotation = DirectionToPlayer.Rotation();

            FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
            SetActorRotation(NewRotation);
        }
    }
}

void AGOJAICharacter::OnAttackAnimationComplete() {}

float AGOJAICharacter::GetDistanceToPlayer() const
{
    const auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Player) return MAX_FLT;

    return FVector::Distance(GetActorLocation(), Player->GetActorLocation());
}

float AGOJAICharacter::GetSmallestAttackRange() const
{
    float SmallestRange = MAX_FLT;

    SmallestRange = FMath::Min(SmallestRange, CombatComponent->GetStrikeInfo(EStrikeType::Light).HitRange);
    SmallestRange = FMath::Min(SmallestRange, CombatComponent->GetStrikeInfo(EStrikeType::Heavy).HitRange);
    SmallestRange = FMath::Min(SmallestRange, CombatComponent->GetStrikeInfo(EStrikeType::Kick).HitRange);

    return SmallestRange;
}

FVector AGOJAICharacter::GetRandomizedDestination(const FVector& TargetLocation, float OffsetRange) const
{
    FVector Offset = FVector(FMath::FRandRange(-OffsetRange, OffsetRange), FMath::FRandRange(-OffsetRange, OffsetRange), 0.0f);

    return TargetLocation + Offset;
}

TArray<EStrikeType> AGOJAICharacter::GetAvailableAttack(float DistanceToPlayer) const
{
    TArray<EStrikeType> AvailableAttacks;

    if (DistanceToPlayer <= CombatComponent->GetStrikeInfo(EStrikeType::Light).HitRange &&                     //
        StaminaComponent->GetStamina() >= CombatComponent->GetStrikeInfo(EStrikeType::Light).RequiredStamina)  //
    {
        AvailableAttacks.Add(EStrikeType::Light);
    }
    if (DistanceToPlayer <= CombatComponent->GetStrikeInfo(EStrikeType::Heavy).HitRange &&                     //
        StaminaComponent->GetStamina() >= CombatComponent->GetStrikeInfo(EStrikeType::Heavy).RequiredStamina)  //
    {
        AvailableAttacks.Add(EStrikeType::Heavy);
    }
    if (DistanceToPlayer <= CombatComponent->GetStrikeInfo(EStrikeType::Kick).HitRange &&                     //
        StaminaComponent->GetStamina() >= CombatComponent->GetStrikeInfo(EStrikeType::Kick).RequiredStamina)  //
    {
        AvailableAttacks.Add(EStrikeType::Kick);
    }

    return AvailableAttacks;
}

EStrikeType AGOJAICharacter::ChooseRandomAttack(const TArray<EStrikeType>& AvailableAttacks) const
{
    TMap<EStrikeType, float> AttackWeights;

    AttackWeights.Add(EStrikeType::Light, 0.6f);
    AttackWeights.Add(EStrikeType::Heavy, 0.3f);
    AttackWeights.Add(EStrikeType::Kick, 0.1f);

    TArray<EStrikeType> FilteredAttacks;

    for (const auto& Attack : AvailableAttacks)
    {
        if (Attack != LastAttackType)
        {
            FilteredAttacks.Add(Attack);
        }
    }

    if (FilteredAttacks.Num() == 0)
    {
        return LastAttackType;
    }

    TArray<float> Weights;
    for (const auto& Attack : FilteredAttacks)
    {
        Weights.Add(AttackWeights[Attack]);
    }

    float TotalWeight = 0.0f;
    for (float Weight : Weights)
    {
        TotalWeight += Weight;
    }

    float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
    float CurrentWeight = 0.0f;

    for (int32 i = 0; i < FilteredAttacks.Num(); i++)
    {
        CurrentWeight += Weights[i];
        if (RandomValue <= CurrentWeight)
        {
            return FilteredAttacks[i];
        }
    }

    return FilteredAttacks[0];
}
