// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GOJMoveToPlayerAndAttackTask.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJMoveToPlayerAndAttackTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UGOJMoveToPlayerAndAttackTask();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 200.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackCooldown = 1.0f;

    float LastAttackTime = 0.0f;

    void PerformAttack(class AAIController* AIController, class AGOJBaseCharacter* OwnerCharacter);
};
