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

        const float RandomChance = FMath::FRand();

        if (StaminaComponent && StaminaComponent->GetStamina() <= StaminaComponent->StaminLevelToRetreat)
        {
            bIsUnderAttack = RandomChance < 0.5f;  
        }

        if (HealthComponent && HealthComponent->GetIsUnderAtack())
        {
            bIsUnderAttack = RandomChance < 0.60f;  
        }

        if (!bIsUnderAttack && RandomChance < 0.3f)
        {
            bIsUnderAttack = true;  
        }

        BlackboardComponent->SetValueAsBool(IsUnderAttackBlackboardName, bIsUnderAttack);

        UE_LOG(LogTemp, Display, TEXT("RandomChance: %f, bIsUnderAttack: %s"), RandomChance, bIsUnderAttack ? TEXT("True") : TEXT("False"));
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
