// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GOJGameHUD.generated.h"

/**
 *
 */
UCLASS()
class GEARSOFJUSTICE_API AGOJGameHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> CombatHUDWidgetClass;

    virtual void BeginPlay() override;


};
