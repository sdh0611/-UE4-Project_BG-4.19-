// Fill out your copyright notice in the Description page of Project Settings.

#include "BGRecovery.h"
#include "BGPlayer.h"
#include "BGHPComponent.h"


ABGRecovery::ABGRecovery()
{
	RecoveryAmount = 30;
	RecoveryDelay = 0.f;
	ItemName = TEXT("Recovery");
}

void ABGRecovery::OnUsed()
{
	UE_LOG(LogClass, Warning, TEXT("Your use recovery item!"));
	
	ItemOwner->GetHPComponent()->AdjustCurrentHP(RecoveryAmount);

	Super::OnUsed();
}

void ABGRecovery::InitItemData(FBGItemData * NewItemData)
{
	if (NewItemData)
	{
		//TODO : ���߿� Custom Cast�� �����ؼ� ��ü�� ��.
		FBGRecoveryData* RecoveryData = static_cast<FBGRecoveryData*>(NewItemData);
		if (RecoveryData)
		{
			RecoveryAmount = RecoveryData->RecoveryAmount;
			RecoveryDelay = RecoveryData->RecoveryDelay;

			Super::InitItemData(NewItemData);
		}
	}

}
