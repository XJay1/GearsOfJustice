// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GOJCoreTypes.h"
#include "GOJAttackTask.generated.h"

class AAIController;
class AGOJAICharacter;

UCLASS()
class GEARSOFJUSTICE_API UGOJAttackTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UGOJAttackTask();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackCooldown = 1.0f;

    float LastAttackTime = 0.0f;


};
