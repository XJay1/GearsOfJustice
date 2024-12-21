// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/GOJFindSaveLocationTask.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogGOJFindSaveLocationTask, All, All);

UGOJFindSaveLocationTask::UGOJFindSaveLocationTask()
{
    NodeName = "Safe Location";
}

EBTNodeResult::Type UGOJFindSaveLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return EBTNodeResult::Failed;

    const auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSystem) return EBTNodeResult::Failed;

    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    FVector RandomPoint;

    bool bFound = false;

    for (int i = 0; i < 10; i++)
    {
        float RandomAngle = FMath::RandRange(0.f, 360.0f);
        float RandomRadius = FMath::RandRange(500.0f, 1000.0f);

        RandomPoint = PlayerLocation + FVector(FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomRadius,  //
                                           FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomRadius,      //
                                           0.f);                                                                 //

        FNavLocation NavLocation;
        if (NavSystem->ProjectPointToNavigation(RandomPoint, NavLocation))
        {
            RandomPoint = NavLocation.Location;
            bFound = true;
            break;
        }
    }

    if (bFound)
    {
        Blackboard->SetValueAsVector("SafeLocation", RandomPoint);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
