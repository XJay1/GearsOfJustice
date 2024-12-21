// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/GOJAttackTask.h"
#include "AIController.h"
#include "AI/GOJAICharacter.h"
#include "Components/GOJCombatComponent.h"
#include "Kismet/GameplayStatics.h"

UGOJAttackTask::UGOJAttackTask() 
{
    NodeName = TEXT("Perform Random Strike");
}

EBTNodeResult::Type UGOJAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    const auto AICharacter = Cast<AGOJAICharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    AICharacter->PerformRandomAttack();

    return EBTNodeResult::Succeeded;
    
}



