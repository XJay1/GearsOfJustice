// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOJCoreTypes.h"
#include "GOJHealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GEARSOFJUSTICE_API UGOJHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGOJHealthComponent();

    FOnHealthChangedSignature OnHealthChanged;
    FOnDeathSignature OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return Health / MaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health) || Health <= 0; }

    float GetHealth() const { return Health; }

    bool IsHealthFull() const;

    UFUNCTION(BlueprintCallable)
    bool GetIsUnderAtack() { return bIsUnderAttack; }

    UFUNCTION(BlueprintCallable)
    void SetIsUnderAtack(bool IsUnderAtack) { bIsUnderAttack = IsUnderAtack; }

protected:
    virtual void BeginPlay() override;

    FTimerHandle UnderAttackTimerHandle;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION()
    void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0"))
    float UnderAttackDelay = 2.0f;

private:
    float Health = 0.0f;
    FTimerHandle HealTimerHandle;

    void SetHealth(float NewHealth);

    void Dead(AController* KillerController);

    bool bIsUnderAttack = false;

    void OnUnderAttackTimerFinished();
};
