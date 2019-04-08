// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "Blueprint/UserWidget.h"
#include "BGUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API UBGUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterHP(class UBGHPComponent* NewHP);
	void BindWeaponInfo(class ABGWeapon* NewWeapon);
	void BindPlayerItemStatus(class UBGPlayerItemStatusComponent* NewItemStatus);

public:
	//void SetVisibleAmmoInfo(bool NewState);

protected:
	virtual void NativeConstruct() override;
	void UpdateHP();
	void UpdateCurrentAmmo();
	void UpdateItemStatus();

private:
	TWeakObjectPtr<class UBGHPComponent> CurrentCharacterHP;
	TWeakObjectPtr<class ABGWeapon> CurrentWeapon;
	TWeakObjectPtr<class UBGPlayerItemStatusComponent> CurrentPlayerItemStatus;

private:
	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* DopingBar;

	UPROPERTY()
	class UTextBlock* CurrentAmmoTxt;
	
	UPROPERTY()
	class UTextBlock* TotalAmmoTxt;

	UPROPERTY()
	class UTextBlock* CurrentMoneyTxt;

};
