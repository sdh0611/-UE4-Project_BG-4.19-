// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/BGItem.h"
#include "BGRecovery.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTBG_419_API ABGRecovery : public ABGItem
{
	GENERATED_BODY()
	
public:
	ABGRecovery();
	
public:
	virtual void OnUsed() override;
	virtual void InitItemData(struct FBGItemData* NewItemData) override;

private:
	float RecoveryAmount;
	float RecoveryDelay;

};
