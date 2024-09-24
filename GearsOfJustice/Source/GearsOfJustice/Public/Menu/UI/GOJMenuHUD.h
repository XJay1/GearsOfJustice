// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GOJMainTypes.h"
#include "GOJMenuHUD.generated.h"


UCLASS()
class GEARSOFJUSTICE_API AGOJMenuHUD : public AHUD
{
    GENERATED_BODY()

public:
    void ChangeMenuWidgetsVisability(EGOJMenuStates ActiveMenuState);
    

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> SettingsWidget;

    virtual void BeginPlay() override;


private:
    UPROPERTY()
    TMap<EGOJMenuStates, UUserWidget*> MenuWidgets;

    UPROPERTY()
    UUserWidget* CurrentMenuWidget = nullptr;
};
