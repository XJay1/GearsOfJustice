// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GOJGameHUD.h"
#include "Blueprint/UserWidget.h"

void AGOJGameHUD::BeginPlay()
{
    Super::BeginPlay();
    CombatWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), CombatHUDWidgetClass);
    if (CombatWidget)
    {
        CombatWidget->AddToViewport();
        CombatWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AGOJGameHUD::ShowCombatHUD()
{
    if (CombatWidget)
    {
        CombatWidget->SetVisibility(ESlateVisibility::Visible);  
    }
}

void AGOJGameHUD::HideCombatHUD()
{
    if (CombatWidget && CombatWidget->IsInViewport())
    {
        CombatWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

