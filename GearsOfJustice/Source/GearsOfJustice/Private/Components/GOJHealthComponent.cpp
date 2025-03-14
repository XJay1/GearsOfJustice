// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/GOJHealthComponent.h"
#include "Player/GOJBaseCharacter.h"
#include "Components/GOJCombatComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGOJHealthComponent, All, All);

UGOJHealthComponent::UGOJHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UGOJHealthComponent::IsHealthFull() const
{
    return Health == MaxHealth;
}

void UGOJHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UGOJHealthComponent::OnTakeAnyDamage);
    }
}

void UGOJHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGOJHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (GetIsDead() || !GetWorld()) return;

    const auto CombatComponent = GetGOJCombatComponent();
    if (!CombatComponent) return;

    CombatComponent->SetZeroHitsWithoutDamageCount();

    SetHealth(Health - Damage);

    if (GetIsDead())
    {
        OnDeath.Broadcast();
        return;
    }

    SetIsUnderAtack(true);
    GetWorld()->GetTimerManager().SetTimer(
        UnderAttackTimerHandle, this, &UGOJHealthComponent::OnUnderAttackTimerFinished, UnderAttackDelay);   
}

void UGOJHealthComponent::SetHealth(float NewHealth)
{
    const float NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const float HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChanged.Broadcast(NextHealth, HealthDelta);
}

void UGOJHealthComponent::Dead(AController* KillerController)
{
    if (!GetWorld()) return;
}

void UGOJHealthComponent::OnUnderAttackTimerFinished()
{
    SetIsUnderAtack(false);
    UE_LOG(LogGOJHealthComponent, Display, TEXT("bIsUnderAttack was changed to: %s"), bIsUnderAttack ? TEXT("True") : TEXT("False"));
}

UGOJCombatComponent* UGOJHealthComponent::GetGOJCombatComponent()
{
    const auto Character = GetOwner();
    if (!Character) return nullptr;

    const auto BaseCharacter = Cast<AGOJBaseCharacter>(Character);
    if (!BaseCharacter) return nullptr;

    return BaseCharacter->FindComponentByClass<UGOJCombatComponent>();
}
