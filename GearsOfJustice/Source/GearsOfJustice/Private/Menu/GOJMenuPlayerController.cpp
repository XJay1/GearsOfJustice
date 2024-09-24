// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/GOJMenuPlayerController.h"

void AGOJMenuPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}
