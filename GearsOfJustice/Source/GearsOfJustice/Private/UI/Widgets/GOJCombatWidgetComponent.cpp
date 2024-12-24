 //Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/GOJCombatWidgetComponent.h"
#include "Components/GOJHealthComponent.h"
#include "Player/GOJBaseCharacter.h"

bool UGOJCombatWidgetComponent::Initialize()
{
    const auto PlayerPawn = GetOwningPlayerPawn();

    if (!PlayerPawn)
    {
        UE_LOG(LogTemp, Display, TEXT("No player pawn"));
        return Super::Initialize();
    }

    const auto BaseCharacter = Cast<AGOJBaseCharacter>(PlayerPawn);
    if (!BaseCharacter)
    {
        UE_LOG(LogTemp, Display, TEXT("No player pawn"));
        return Super::Initialize();
    }

    const auto HealthComponent = BaseCharacter->FindComponentByClass<UGOJHealthComponent>();
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &UGOJCombatWidgetComponent::OnHealthChanged);
    }

    return Super::Initialize();
}

void UGOJCombatWidgetComponent::OnHealthChanged(float Health, float Delta)
{
    if (Delta < 0)  
    {
        OnTakeDamage();  
    }
}
