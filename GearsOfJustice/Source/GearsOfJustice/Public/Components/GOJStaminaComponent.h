// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOJCoreTypes.h"
#include "GOJStaminaComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GEARSOFJUSTICE_API UGOJStaminaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGOJStaminaComponent();

    FOnStaminaChangedSignature OnStaminaChangedDelegate;

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    float GetStaminaPercent() const { return Stamina / MaxStamina; }

    UFUNCTION(BlueprintCallable, Category = "Stamina")
    float GetStamina() const { return Stamina; }
    UFUNCTION(BlueprintCallable, Category = "Stamina")
    bool IsStaminaFull() const { return Stamina == MaxStamina; }

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    float StaminLevelToRetreat = 30.0f;

    bool GetAutoStaminaRegen() const { return AutoStaminaRegen; }
    void SetStaminaAutoRegen(bool StaminaAutoRegen) { AutoStaminaRegen = StaminaAutoRegen; } 

    bool HasEnoughStamina(float RequiredStamina) const { return Stamina >= RequiredStamina; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float MaxStamina = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    bool AutoStaminaRegen = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaUpdateDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaUpdateTime = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaModifier = 5.0f;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void IncreaseStamina(float RequiredStamina);

private:
    float Stamina = 0.0f;

    void SetStamina(float NewStamina);

    void UpdateStamina();
    FTimerHandle StaminaTimerHandle;
};
