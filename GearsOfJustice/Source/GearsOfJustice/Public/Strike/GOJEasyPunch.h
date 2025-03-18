// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strike/GOJBasicStrike.h"
#include "GOJEasyPunch.generated.h"

UCLASS()
class GEARSOFJUSTICE_API AGOJEasyPunch : public AGOJBasicStrike
{
    GENERATED_BODY()

    AGOJEasyPunch()
    {
        StrikeInfo.Damage = 10.0f;
        StrikeInfo.RequiredStamina = 15.0f;
        StrikeInfo.HitRange = 200.0f;
    }
};
