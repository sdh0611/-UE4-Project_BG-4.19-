// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/BGPickup.h"
#include "BGRecoveryPickup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API ABGRecoveryPickup : public ABGPickup
{
	GENERATED_BODY()
	
public:
	ABGRecoveryPickup();

public:
	virtual void BeginPlay() override;
	virtual void OnInteraction(class APawn* Pawn) override;

	
	
};
