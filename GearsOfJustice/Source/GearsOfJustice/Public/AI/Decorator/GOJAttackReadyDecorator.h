// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "GOJAttackReadyDecorator.generated.h"

class AGOJAICharacter;

UCLASS()
class GEARSOFJUSTICE_API UGOJAttackReadyDecorator : public UBTDecorator_BlackboardBase
{
    GENERATED_BODY()

public:
    UGOJAttackReadyDecorator();

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

private:
    bool CanAttack(AAIController* AIController, AGOJAICharacter* AICharacter, float DistanceToPlayer) const;
};
