#include "AI/Task/GOJMoveToPlayerTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/GOJAICharacter.h"
#include "Kismet/GameplayStatics.h"

UGOJMoveToPlayerTask::UGOJMoveToPlayerTask()
{
    NodeName = "Move to Player";
    bNotifyTick = true;  // Включаем возможность вызывать TickTask
}

EBTNodeResult::Type UGOJMoveToPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Получаем AIController
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    // Получаем персонажа ИИ
    AGOJAICharacter* AICharacter = Cast<AGOJAICharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    // Получаем игрока
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(AICharacter->GetWorld(), 0);
    if (!PlayerCharacter) return EBTNodeResult::Failed;

    // Рассчитываем дистанцию до игрока
    float DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());

    // Проверяем, находится ли ИИ на нужной дистанции для атаки
    const float AdjustedAttackRange = AICharacter->GetSmallestAttackRange() - 10.0f;  // Уменьшаем радиус на 10 единиц
    if (DistanceToPlayer <= AdjustedAttackRange)
    {
        AIController->StopMovement();
        return EBTNodeResult::Succeeded;
    }

    // Создаем запрос на движение к игроку
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(PlayerCharacter);
    MoveRequest.SetAcceptanceRadius(AdjustedAttackRange);  // Устанавливаем скорректированный радиус

    // Пытаемся выполнить движение
    FNavPathSharedPtr NavPath;
    EPathFollowingRequestResult::Type MoveResult = AIController->MoveTo(MoveRequest, &NavPath);

    if (MoveResult == EPathFollowingRequestResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::InProgress;
}

void UGOJMoveToPlayerTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // Получаем AIController
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Получаем персонажа ИИ
    AGOJAICharacter* AICharacter = Cast<AGOJAICharacter>(AIController->GetPawn());
    if (!AICharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Получаем игрока
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(AICharacter->GetWorld(), 0);
    if (!PlayerCharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Рассчитываем дистанцию до игрока
    float DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());

    // Проверяем, достиг ли ИИ скорректированной дистанции
    const float AdjustedAttackRange = AICharacter->GetSmallestAttackRange() - 10.0f;  // Уменьшаем радиус на 10 единиц
    if (DistanceToPlayer <= AdjustedAttackRange)
    {
        AIController->StopMovement();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // Проверяем статус движения
    const auto PathFollowingComponent = AIController->GetPathFollowingComponent();
    if (PathFollowingComponent && PathFollowingComponent->GetStatus() == EPathFollowingStatus::Idle)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
}
