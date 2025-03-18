// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/GOJAttackReadyDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "AI/GOJAICharacter.h"
#include "Kismet/GameplayStatics.h"

UGOJAttackReadyDecorator::UGOJAttackReadyDecorator() 
{
    NodeName = "Can Attack Player";
}

bool UGOJAttackReadyDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController) return false;

    const auto AICharacter = Cast<AGOJAICharacter>(AIController->GetPawn());
    if (!AICharacter) return false;

    const auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(AICharacter->GetWorld(), 0);
    if (!PlayerCharacter) return false;

    float DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());

    return CanAttack(AIController, AICharacter, DistanceToPlayer);
    ;
}

bool UGOJAttackReadyDecorator::CanAttack(AAIController* AIController, AGOJAICharacter* AICharacter, float DistanceToPlayer) const
{
    TArray<EStrikeType> AvailableAttacks = AICharacter->GetAvailableAttack(DistanceToPlayer);

    return AvailableAttacks.Num() > 0;
}
