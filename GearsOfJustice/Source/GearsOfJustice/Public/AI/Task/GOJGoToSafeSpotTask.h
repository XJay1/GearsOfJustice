// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GOJGoToSafeSpotTask.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJGoToSafeSpotTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UGOJGoToSafeSpotTask();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    AActor* PlayerPawn;     // Ссылка на игрока
    APawn* ControlledPawn;  // Ссылка на ИИ
    FVector SafeLocation;   // Целевая точка

};
