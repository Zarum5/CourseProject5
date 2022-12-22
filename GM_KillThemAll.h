// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "GM_KillThemAll.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AGM_KillThemAll : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PawnKilled(APawn* PawnKilled) override;

private:
	void EndGame(bool bIsPlayerWinner);
};
