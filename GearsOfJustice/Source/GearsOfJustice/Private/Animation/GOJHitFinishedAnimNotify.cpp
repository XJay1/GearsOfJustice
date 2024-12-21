// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GOJHitFinishedAnimNotify.h"
#include "GameFramework/Actor.h"
#include "AI/GOJAICharacter.h" 

void UGOJHitFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
    if (MeshComp && MeshComp->GetOwner())
    {
        const auto AICharacter = Cast<AGOJAICharacter>(MeshComp->GetOwner());
        if (AICharacter)
        {
            AICharacter->OnAttackAnimationComplete();
        }
    }
}
