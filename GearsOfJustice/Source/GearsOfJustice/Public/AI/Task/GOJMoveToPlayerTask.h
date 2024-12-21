// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GOJMoveToPlayerTask.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJMoveToPlayerTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UGOJMoveToPlayerTask();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "AI")
    float RequiredDistance = 200.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float TimeOut = 10.0f;

private:
    FTimerHandle TimerHandle;
    FVector LastPlayerPosition;
};
