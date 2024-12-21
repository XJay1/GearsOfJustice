// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GOJBlockTask.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJBlockTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UGOJBlockTask();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
