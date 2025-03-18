// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/GOJCheckStaminaDecorator.h"
#include "AIController.h"
#include "AI/GOJAICharacter.h"
#include "Components/GOJStaminaComponent.h"

UGOJCheckStaminaDecorator::UGOJCheckStaminaDecorator() 
{
    NodeName = "Check Stamina";
}

bool UGOJCheckStaminaDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto GOJAICharacter = Cast<AGOJAICharacter>(Controller->GetPawn());
    if (!GOJAICharacter) return false;

    const auto StaminaComponent = GOJAICharacter->FindComponentByClass<UGOJStaminaComponent>();
    if (StaminaComponent && StaminaComponent->GetStamina() > StaminaThreshold) return true;

    return false;
}


