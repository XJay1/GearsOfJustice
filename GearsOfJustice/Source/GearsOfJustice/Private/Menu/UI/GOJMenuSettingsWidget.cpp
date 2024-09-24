// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/GOJMenuSettingsWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "GOJMainTypes.h"
#include "Menu/UI/GOJMenuHUD.h"

void UGOJMenuSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ToMainMenuButton)
    {
        ToMainMenuButton->OnClicked.AddDynamic(this, &UGOJMenuSettingsWidget::OnToMainMenuButtonClick);
    }
}

void UGOJMenuSettingsWidget::OnToMainMenuButtonClick()
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (!PlayerController) return;

    AHUD* HUD = PlayerController->GetHUD();
    if (!HUD) return;

    AGOJMenuHUD* MenuHUD = Cast<AGOJMenuHUD>(HUD);
    if (!MenuHUD) return;

    MenuHUD->ChangeMenuWidgetsVisability(EGOJMenuStates::MainMenu);
}
