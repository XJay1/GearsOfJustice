// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strike/GOJBasicStrike.h"
#include "GOJStrongPunch.generated.h"


UCLASS()
class GEARSOFJUSTICE_API AGOJStrongPunch : public AGOJBasicStrike
{
	GENERATED_BODY()
	

	AGOJStrongPunch()
    {
        StrikeInfo.Damage = 30.0f;
        StrikeInfo.RequiredStamina = 40.0f;
        StrikeInfo.HitRange = 200.0f;
    }
};
