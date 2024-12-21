// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOJAIController.h"
#include "AI/GOJAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


AGOJAIController::AGOJAIController()
{
    
}

void AGOJAIController::OnPossess(APawn* InPawn) 
{
    Super::OnPossess(InPawn);

    if (const auto GOJCharacter = Cast<AGOJAICharacter>(InPawn))
    {
        RunBehaviorTree(GOJCharacter->BehaviorTreeAsset);
    }
}
