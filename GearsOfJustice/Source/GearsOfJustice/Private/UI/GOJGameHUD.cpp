// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOJGameHUD.h"
#include "Blueprint/UserWidget.h"

void AGOJGameHUD::BeginPlay()
{
    Super::BeginPlay();
    auto CombatHUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), CombatHUDWidgetClass);
    if (CombatHUDWidget)
    {
        CombatHUDWidget->AddToViewport();
    }
}
