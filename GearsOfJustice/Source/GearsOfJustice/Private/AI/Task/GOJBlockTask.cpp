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

    const float RandomDelay = FMath::RandRange(0.5f, 3.0f);

    FTimerManager& TimerManager = Controller->GetWorldTimerManager();

    TimerManager.SetTimer(BlockTimerHandle,
        FTimerDelegate::CreateLambda(
            [this, &OwnerComp, AICharacter]()
            {
                if (AICharacter)
                {
                    AICharacter->OnStopBlocking();
                }
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            }),
        RandomDelay, false);

    return EBTNodeResult::InProgress;
}

void UGOJBlockTask::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    if (FTimerManager* TimerManager = &OwnerComp.GetWorld()->GetTimerManager())
    {
        TimerManager->ClearTimer(BlockTimerHandle);

        const auto Controller = OwnerComp.GetAIOwner();
        if (!Controller) return;

        const auto AICharacter = Cast<AGOJAICharacter>(Controller->GetPawn());
        if (!AICharacter) return;

        AICharacter->OnStopBlocking();
    }

    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}