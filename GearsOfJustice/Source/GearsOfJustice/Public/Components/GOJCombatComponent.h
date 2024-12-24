// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOJCoreTypes.h"
#include "GOJCombatComponent.generated.h"

class AGOJBasicStrike;
class AGOJBaseCharacter;
class UGOJStaminaComponent;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* HitReactionAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* BlockAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* HitInBlockReactionAnimation;

    void PerformStrike(EStrikeType StrikeType);

    void PlayHitReaction();

    FStrikeInfo GetStrikeInfo(EStrikeType StrikeType);

    void StopBlocking();
    void StartBlocking();

    bool GetIsBlocking() const { return bIsBlocking; }
    void SetIsBlocking(bool IsBlocking) { bIsBlocking = IsBlocking; }

    bool GetCanMakeHit() const { return bCanMakeHit; }
    void SetCanMakeHit(bool CanMakeHit) { bCanMakeHit = CanMakeHit; }

    bool GetCanBlock() const { return bCanBlock; }
    void SetCanBlock(bool CanBlock) { bCanBlock = CanBlock; }

    bool GetIsUnderHit() const { return bIsUnderHit; }
    void SetIsUnderHit(bool IsUnderHit) { bIsUnderHit = IsUnderHit; }

private:
    bool bIsBlocking = false;
    bool bCanMakeHit = true;
    bool bCanBlock = true;
    bool bIsUnderHit = false;

    USkeletalMeshComponent* GetCharacterSkeletalMeshComponent();
    AGOJBaseCharacter* GetGOJBaseCharacter();
    UGOJStaminaComponent* GetGOJStaminaComponent();
    UFUNCTION()
    void OnHitMontageEnded(UAnimMontage* Montage, bool bInerrupted);
};
