// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "CustomUserWidget.h"
#include "Kismet/GameplayStatics.h"


void AShooterPlayerController::BeginPlay()
{
    EndScreenIsSet = false;

    UUserWidget* StartScreen = CreateWidget(this,StartScreenClass);
    if (StartScreen) {
        StartScreen->AddToViewport();
    }

    HUD = Cast<UCustomUserWidget>(CreateWidget(this,HUDClass));
    if (HUD) {
        HUD->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus,bIsWinner);

    HUD->CloseWidget();

    if (!EndScreenIsSet) 
    {
        if (bIsWinner)
        {
            UUserWidget* WinScreen = CreateWidget(this,WinScreenClass);
            if (WinScreen) {
                WinScreen->AddToViewport();
                EndScreenIsSet = true;
                UGameplayStatics::SpawnSoundAtLocation(GetWorld(),WinSound,FVector(0,0,0));
            } 
        } else {
            UUserWidget* LoseScreen = CreateWidget(this,LoseScreenClass);
            if (LoseScreen) {
                LoseScreen->AddToViewport();
                EndScreenIsSet = true;
                UGameplayStatics::SpawnSoundAtLocation(GetWorld(),LoseSound,FVector(0,0,0));
            } 
        }
    }
    GetWorldTimerManager().SetTimer(RestartTimer,this,&APlayerController::RestartLevel,RestartDelay);
}


float AShooterPlayerController::GetRestartDelay() const
{
    return RestartDelay;
}

