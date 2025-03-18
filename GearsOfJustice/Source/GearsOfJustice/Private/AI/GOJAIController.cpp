// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GOJAIController.h"
#include "AI/GOJAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"


AGOJAIController::AGOJAIController()
{
    bIsInCombat = false;
}

void AGOJAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AGOJAIController::SetCombatState(bool bInCombat)
{
    bIsInCombat = bInCombat;

    if (const auto GOJCharacter = Cast<AGOJAICharacter>(GetPawn()))
    {
        if (bIsInCombat)
        {
            if (GOJCharacter->BehaviorTreeAsset)
            {
                RunBehaviorTree(GOJCharacter->BehaviorTreeAsset);
            }
        }
        else
        {
            BrainComponent->StopLogic(TEXT("Player left combat zone"));
        }
    }
}
