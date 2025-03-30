// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GOJCombatWidgetComponent.generated.h"

class UGOJHealthComponent;

UCLASS()
class GEARSOFJUSTICE_API UGOJCombatWidgetComponent : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual bool Initialize() override;

    UFUNCTION(BlueprintImplementableEvent)
    void OnTakeDamage();

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    void OnDead();

private:
    void OnHealthChanged(float Health, float Delta);

    UGOJHealthComponent* GetGOJHealthComponent() const;
};
