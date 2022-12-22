// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_KillThemAll.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AGM_KillThemAll::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);


    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController)
    {
        EndGame(false);
    }

    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!Controller->IsDead()) return;
    }

    EndGame(true);
}

void AGM_KillThemAll::EndGame(bool bIsPlayerWinner)
{
    //check for game end condition and pass to controller
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
