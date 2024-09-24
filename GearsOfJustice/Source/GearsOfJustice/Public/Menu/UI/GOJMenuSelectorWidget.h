// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GOJMenuSelectorWidget.generated.h"

class UButton;

UCLASS()
class GEARSOFJUSTICE_API UGOJMenuSelectorWidget : public UUserWidget
{
    GENERATED_BODY()

protected:

    UPROPERTY(meta = (BindWidget))
    UButton* ContinueGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ExitGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SettingsGameButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnContinueGame();

    UFUNCTION()
    void OnExitGame();

    UFUNCTION()
    void OnSettingsClick();
};
