// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GOJStaminaComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogGOJStaminaComponent, All, All);

UGOJStaminaComponent::UGOJStaminaComponent()
{
    SetStamina(MaxStamina);
	PrimaryComponentTick.bCanEverTick = true;
}


void UGOJStaminaComponent::BeginPlay()
{
    SetStamina(MaxStamina);

	Super::BeginPlay();
}


void UGOJStaminaComponent::SetStamina(float NewStamina) 
{
    NewStamina = FMath::Clamp(NewStamina, 0, MaxStamina);

	Stamina = NewStamina;

	OnStaminaChangedDelegate.Broadcast(NewStamina);
}

void UGOJStaminaComponent::UpdateStamina() 
{
    SetStamina(Stamina + StaminaModifier);

	if (IsStaminaFull() && GetWorld())
	{
        GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
	}
}

void UGOJStaminaComponent::IncreaseStamina(float RequiredStamina)
{
    if (Stamina == 0 || !GetWorld()) return;

	UE_LOG(LogGOJStaminaComponent, Display, TEXT("Stamina reduced on %f"), RequiredStamina);

	SetStamina(Stamina - RequiredStamina);

    GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
    if (AutoStaminaRegen)
    {
        GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &UGOJStaminaComponent::UpdateStamina, StaminaUpdateTime, true, StaminaUpdateDelay);
    }

}

void UGOJStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

