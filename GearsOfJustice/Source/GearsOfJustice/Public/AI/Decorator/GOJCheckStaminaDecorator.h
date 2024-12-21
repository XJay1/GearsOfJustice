// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GOJCheckStaminaDecorator.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJCheckStaminaDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    UGOJCheckStaminaDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float StaminaThreshold = 20.0f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
