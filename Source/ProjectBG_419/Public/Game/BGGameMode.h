// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "GameFramework/GameModeBase.h"
#include "BGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API ABGGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	ABGGameMode();

public:
	virtual void BeginPlay() override;

};
