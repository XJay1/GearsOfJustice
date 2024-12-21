// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GOJAIController.generated.h"

UCLASS()
class GEARSOFJUSTICE_API AGOJAIController : public AAIController
{
    GENERATED_BODY()

public:
    AGOJAIController();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTree;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UBlackboardComponent* BlackboardComponent;

    virtual void OnPossess(APawn* InPawn) override;
};
