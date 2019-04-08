// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "GameFramework/Actor.h"
#include "BGInteractionActor.generated.h"

UCLASS()
class PROJECTBG_419_API ABGInteractionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABGInteractionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnInteraction(class APawn* Pawn);
	virtual void OnFocus();
	virtual void OnFocusEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	
};
