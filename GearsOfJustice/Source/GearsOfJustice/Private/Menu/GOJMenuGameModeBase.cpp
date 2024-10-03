// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/GOJMenuGameModeBase.h"
#include "Menu/GOJMenuPlayerController.h"
#include "Menu/UI/GOJMenuHUD.h"


AGOJMenuGameModeBase::AGOJMenuGameModeBase()
{
    PlayerControllerClass = AGOJMenuPlayerController::StaticClass();
    HUDClass = AGOJMenuHUD::StaticClass();
}