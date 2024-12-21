// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/GOJIsUnderAttackService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/GOJAICharacter.h"
#include "Components/GOJHealthComponent.h"
#include "Components/GOJStaminaComponent.h"

UGOJIsUnderAttackService::UGOJIsUnderAttackService() 
{
    NodeName = "IsUnderAttack";
}

void UGOJIsUnderAttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return;

    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto AICharacter = Cast<AGOJAICharacter>(Controller->GetPawn());

    if (BlackboardComponent && AICharacter)
    {
        const auto StaminaComponent = AICharacter->FindComponentByClass<UGOJStaminaComponent>();
        const auto HealthComponent = AICharacter->FindComponentByClass<UGOJHealthComponent>();

        bool bIsUnderAttack = false;

        if (StaminaComponent && StaminaComponent->GetStamina() <= StaminaComponent->StaminLevelToRetreat)
        {
            bIsUnderAttack = true;
        }

        if (HealthComponent && HealthComponent->GetIsUnderAtack())
        {
            bIsUnderAttack = true;
        }

        BlackboardComponent->SetValueAsBool(IsUnderAttackBlackboardName, bIsUnderAttack);
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
