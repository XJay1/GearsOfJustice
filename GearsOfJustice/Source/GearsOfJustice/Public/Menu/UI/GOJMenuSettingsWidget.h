// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GOJMenuSettingsWidget.generated.h"

class UButton;

UCLASS()
class GEARSOFJUSTICE_API UGOJMenuSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ToMainMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnToMainMenuButtonClick();
};
