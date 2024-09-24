// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/GOJMenuHUD.h"
#include "Blueprint/UserWidget.h"



void AGOJMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    MenuWidgets.Add(EGOJMenuStates::MainMenu, CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass));
    MenuWidgets.Add(EGOJMenuStates::SettingsMenu, CreateWidget<UUserWidget>(GetWorld(), SettingsWidget));

    for (auto WidgetPaar: MenuWidgets)
    {
        const auto MenuWidget = WidgetPaar.Value;
        if (!MenuWidget) continue;

        MenuWidget->AddToViewport();
        MenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    ChangeMenuWidgetsVisability(EGOJMenuStates::MainMenu);
}

void AGOJMenuHUD::ChangeMenuWidgetsVisability(EGOJMenuStates ActiveMenuState)
{
    if (CurrentMenuWidget)
    {
        CurrentMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (MenuWidgets.Contains(ActiveMenuState))
    {
        CurrentMenuWidget = MenuWidgets[ActiveMenuState];
    }

    if (CurrentMenuWidget)
    {
        CurrentMenuWidget->SetVisibility(ESlateVisibility::Visible);
    }
}