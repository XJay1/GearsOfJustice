// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Service/GOJFindSaveLocationService.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGOJFindSaveLocationService, All, All);

UGOJFindSaveLocationService::UGOJFindSaveLocationService()
{
    NodeName = "FindSaveLocation";
}

void UGOJFindSaveLocationService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return;

    const auto AICharacter = Controller->GetPawn();
    const auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (AICharacter && PlayerPawn && Blackboard)
    {
        FVector RetreatDirection = AICharacter->GetActorLocation() - PlayerPawn->GetActorLocation();
        RetreatDirection.Normalize();

        FVector SafeSpot = AICharacter->GetActorLocation() + RetreatDirection * 500.0f;  // TODO убарть хардкод
        Blackboard->SetValueAsVector(BlackboarSafeLocationNode, SafeSpot);

        UE_LOG(LogGOJFindSaveLocationService, Display, TEXT("Safe location: %s"), *SafeSpot.ToString());
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
