// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GOJFindSaveLocationTask.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJFindSaveLocationTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UGOJFindSaveLocationTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float Radius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector SafeLocationKey;

    // FName SafeLocationBlackboardName = TEXT("SafeLocation");
};
