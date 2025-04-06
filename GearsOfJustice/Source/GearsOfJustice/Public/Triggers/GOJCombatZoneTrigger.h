// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOJCoreTypes.h"
#include "GOJCombatZoneTrigger.generated.h"

class UBoxComponent;
class UGOJCombatWidgetComponent;

UCLASS()
class GEARSOFJUSTICE_API AGOJCombatZoneTrigger : public AActor
{
    GENERATED_BODY()

public:
    AGOJCombatZoneTrigger();

protected:
    virtual void BeginPlay() override;

private:
    bool bIsBattleOver = false;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* TriggerBox;

    UPROPERTY()
    UGOJCombatWidgetComponent* CombatWidgetComponent;

    UFUNCTION()
    void OnEnterCombatZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnExitCombatZone(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
