// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/GOJAnimNotify.h"
#include "GOJHitFinishedAnimNotify.generated.h"

UCLASS()
class GEARSOFJUSTICE_API UGOJHitFinishedAnimNotify : public UGOJAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
