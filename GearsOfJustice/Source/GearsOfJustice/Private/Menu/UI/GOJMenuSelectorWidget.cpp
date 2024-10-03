// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/GOJMenuSelectorWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GOJMainTypes.h"
#include "Menu/UI/GOJMenuHUD.h"
#include "Kismet/KismetSystemLibrary.h"

void UGOJMenuSelectorWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ContinueGameButton)
    {
        ContinueGameButton->OnClicked.AddDynamic(this, &UGOJMenuSelectorWidget::OnContinueGame);
    }

    if (ExitGameButton)
    {
        ExitGameButton->OnClicked.AddDynamic(this, &UGOJMenuSelectorWidget::OnExitGame);
    }

    if (SettingsGameButton)
    {
        SettingsGameButton->OnClicked.AddDynamic(this, &UGOJMenuSelectorWidget::OnSettingsClick);
    }
}

void UGOJMenuSelectorWidget::OnContinueGame()
{
    const FName StartupLevelName = "TopDownMap";
    UGameplayStatics::OpenLevel(this, StartupLevelName);
}

void UGOJMenuSelectorWidget::OnExitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UGOJMenuSelectorWidget::OnSettingsClick()
{

    APlayerController* PlayerController = GetOwningPlayer();
    if (!PlayerController) return;

    AHUD* HUD = PlayerController->GetHUD();
    if (!HUD) return;

    AGOJMenuHUD* MenuHUD = Cast<AGOJMenuHUD>(HUD);
    if (!MenuHUD) return;

    MenuHUD->ChangeMenuWidgetsVisability(EGOJMenuStates::SettingsMenu);
}
