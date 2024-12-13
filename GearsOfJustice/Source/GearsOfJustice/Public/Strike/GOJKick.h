// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strike/GOJBasicStrike.h"
#include "GOJKick.generated.h"


UCLASS()
class GEARSOFJUSTICE_API AGOJKick : public AGOJBasicStrike
{
	GENERATED_BODY()
	
	AGOJKick()
    {
        StrikeInfo.Damage = 20.0f;
        StrikeInfo.RequiredStamina = 25.0f;
        StrikeInfo.HitRange = 200.0f;
    }
};
