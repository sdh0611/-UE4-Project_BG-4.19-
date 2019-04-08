// Fill out your copyright notice in the Description page of Project Settings.

#include "BGInteractionActor.h"
//#include "GameFramework/Pawn.h"

// Sets default values
ABGInteractionActor::ABGInteractionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABGInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABGInteractionActor::OnInteraction(APawn* Pawn)
{
}

void ABGInteractionActor::OnFocus()
{
}

void ABGInteractionActor::OnFocusEnd()
{
}

// Called every frame
void ABGInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

