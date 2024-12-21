// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GOJGetCharacterLocationService.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJGetCharacterLocationService : public UBTService
{
    GENERATED_BODY()

    UGOJGetCharacterLocationService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
