// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOJCoreTypes.h"
#include "GOJCombatComponent.generated.h"

class AGOJBasicStrike;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GEARSOFJUSTICE_API UGOJCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGOJCombatComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<AGOJBasicStrike> LightStrikeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<AGOJBasicStrike> HeavyStrikeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<AGOJBasicStrike> KickStrikeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* HitReactionAnimation;

    void PerformStrike(EStrikeType StrikeType);

    void PlayHitReaction();

};
