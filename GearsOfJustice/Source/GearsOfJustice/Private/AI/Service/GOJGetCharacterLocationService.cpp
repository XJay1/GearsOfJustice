// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Service/GOJGetCharacterLocationService.h"
#include "AIController.h"
#include "AI/GOJAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY_STATIC(LogGOJGetCharacterLocationServiceLog, All, All);

UGOJGetCharacterLocationService::UGOJGetCharacterLocationService()
{
    NodeName = "FindEnemy";
}

void UGOJGetCharacterLocationService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return;

    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto AICharacter = Cast<AGOJAICharacter>(Controller->GetPawn());

    if (BlackboardComponent && AICharacter)
    {

        const auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        if (PlayerPawn)
        {
            BlackboardComponent->SetValueAsVector("PlayerLocation", PlayerPawn->GetActorLocation());
        }

      

        UE_LOG(LogGOJGetCharacterLocationServiceLog, Display, TEXT("ShouldRetreat: %s"),
            BlackboardComponent->GetValueAsBool("ShouldRetreat") ? TEXT("True") : TEXT("False"));
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
