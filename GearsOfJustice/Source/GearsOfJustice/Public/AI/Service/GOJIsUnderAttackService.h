// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GOJIsUnderAttackService.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJIsUnderAttackService : public UBTService
{
    GENERATED_BODY()

public:
    UGOJIsUnderAttackService();

protected:
    FName IsUnderAttackBlackboardName = TEXT("IsUnderAttack");

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
