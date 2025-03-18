// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GOJFindSaveLocationService.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJFindSaveLocationService : public UBTService
{
    GENERATED_BODY()

public:
    UGOJFindSaveLocationService();

protected:
    FName BlackboarSafeLocationNode = TEXT("SafeLocation");

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
