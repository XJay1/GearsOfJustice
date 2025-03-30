#include "Triggers/GOJCombatZoneTrigger.h"
#include "Components/BoxComponent.h"
#include "Player/GOJBaseCharacter.h"
#include "AI/GOJAIController.h"
#include "AI/GOJAICharacter.h"
#include "BrainComponent.h"
#include "EngineUtils.h"  
#include "GOJGameHUD.h"
#include "Components/GOJHealthComponent.h"
#include "UI/Widgets/GOJCombatWidgetComponent.h" 

AGOJCombatZoneTrigger::AGOJCombatZoneTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerBox->SetupAttachment(RootComponent);

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGOJCombatZoneTrigger::OnEnterCombatZone);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGOJCombatZoneTrigger::OnExitCombatZone);
}

void AGOJCombatZoneTrigger::BeginPlay()
{
    Super::BeginPlay();

    
}

void AGOJCombatZoneTrigger::OnEnterCombatZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AGOJBaseCharacter* Character = Cast<AGOJBaseCharacter>(OtherActor))
    {
        Character->SetCharacterState(ECharacterState::Combat);

        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            AGOJGameHUD* GameHUD = Cast<AGOJGameHUD>(PlayerController->GetHUD());
            if (GameHUD)
            {
                GameHUD->ShowCombatHUD();  
            }
        }

        for (TActorIterator<AGOJAICharacter> It(GetWorld()); It; ++It)
        {
            AGOJAICharacter* AICharacter = *It;
            if (AICharacter)
            {
                AGOJAIController* AIController = Cast<AGOJAIController>(AICharacter->GetController());
                if (AIController)
                {
                    AIController->RunBehaviorTree(AICharacter->BehaviorTreeAsset);
                }
            }
        }
    }
}

void AGOJCombatZoneTrigger::OnExitCombatZone(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AGOJBaseCharacter* Character = Cast<AGOJBaseCharacter>(OtherActor))
    {
        Character->SetCharacterState(ECharacterState::Exploring);

        const auto HealthComponent = Character->GetComponentByClass<UGOJHealthComponent>();
        if (!HealthComponent) return;

        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            AGOJGameHUD* GameHUD = Cast<AGOJGameHUD>(PlayerController->GetHUD());
            if (GameHUD)
            {
                if (HealthComponent->GetIsDead())
                {

                }
                else
                {
                    GameHUD->HideCombatHUD();
                }
            }
        }

        for (TActorIterator<AGOJAICharacter> It(GetWorld()); It; ++It)
        {
            AGOJAICharacter* AICharacter = *It;
            if (AICharacter)
            {
                AGOJAIController* AIController = Cast<AGOJAIController>(AICharacter->GetController());
                if (AIController && AIController->BrainComponent)
                {
                    AIController->BrainComponent->StopLogic(TEXT("Player left combat zone"));
                }
            }
        }
    }
}
