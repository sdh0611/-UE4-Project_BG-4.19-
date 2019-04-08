// Fill out your copyright notice in the Description page of Project Settings.

#include "BGGameMode.h"
#include "BGPlayer.h"
#include "BGPlayerController.h"
//#include "BGCrosshair.h"
#include "BGHUD.h"
#include "BGUserWidget.h"
#include "ConstructorHelpers.h"

ABGGameMode::ABGGameMode()
{
	DefaultPawnClass = ABGPlayer::StaticClass();
	PlayerControllerClass = ABGPlayerController::StaticClass();
	HUDClass = ABGHUD::StaticClass();

	//static ConstructorHelpers::FClassFinder<ABGPlayer>
	//	BP_BGPLAYER(TEXT("Blueprint'/Game/Blueprints/BGPlayer_BP.BGPlayer_BP_C'"));
	//if (BP_BGPLAYER.Succeeded())
	//{
	//	DefaultPawnClass = BP_BGPLAYER.Class;
	//}

	

}

void ABGGameMode::BeginPlay()
{
	Super::BeginPlay();

}
