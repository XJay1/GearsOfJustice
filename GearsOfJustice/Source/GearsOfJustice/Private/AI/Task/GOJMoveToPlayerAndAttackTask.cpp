// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/GOJMoveToPlayerAndAttackTask.h"
#include "AIController.h"
#include "AI/GOJAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/GOJCombatComponent.h"
#include "Engine/World.h"
#include "NavigationSystem.h"

UGOJMoveToPlayerAndAttackTask::UGOJMoveToPlayerAndAttackTask() 
{
    NodeName = TEXT("Move To Player And Attack");
}

EBTNodeResult::Type UGOJMoveToPlayerAndAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Получаем AIController и персонажа
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AGOJBaseCharacter* OwnerCharacter = Cast<AGOJBaseCharacter>(AIController->GetPawn());
    if (!OwnerCharacter) return EBTNodeResult::Failed;

    // Получаем ссылку на игрока
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return EBTNodeResult::Failed;

    // Вычисляем расстояние до игрока
    float DistanceToPlayer = FVector::Dist(OwnerCharacter->GetActorLocation(), PlayerPawn->GetActorLocation());

    // Если ИИ достаточно близко, выполняем удар
    if (DistanceToPlayer <= AttackRange)
    {
        // Проверка на cooldown удара
        if (GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackCooldown)
        {
            // Выполняем удар
            PerformAttack(AIController, OwnerCharacter);

            // Устанавливаем время последней атаки
            LastAttackTime = GetWorld()->GetTimeSeconds();

            return EBTNodeResult::Succeeded;
        }
    }
    else
    {
        // Если ИИ слишком далеко, продолжаем движение
        AIController->MoveToActor(PlayerPawn);
    }

    return EBTNodeResult::InProgress;
}

void UGOJMoveToPlayerAndAttackTask::PerformAttack(AAIController* AIController, AGOJBaseCharacter* OwnerCharacter)
{
    UGOJCombatComponent* CombatComponent = OwnerCharacter->FindComponentByClass<UGOJCombatComponent>();
    if (CombatComponent)
    {
        // Выбираем случайный тип удара
        EStrikeType RandomStrike = static_cast<EStrikeType>(FMath::RandRange(0, 2));
        CombatComponent->PerformStrike(RandomStrike);
    }
}
