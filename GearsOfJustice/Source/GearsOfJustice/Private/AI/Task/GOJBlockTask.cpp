// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/GOJBlockTask.h"
#include "AIController.h"
#include "AI/GOJAICharacter.h"


UGOJBlockTask::UGOJBlockTask()
{
    NodeName = "StartBlocking";
}

EBTNodeResult::Type UGOJBlockTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    const auto AICharacter = Cast<AGOJAICharacter>(Controller->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    UE_LOG(LogTemp, Display, TEXT("Start blocking"));

    AICharacter->OnStartBlocking();

    return EBTNodeResult::Succeeded;
}
