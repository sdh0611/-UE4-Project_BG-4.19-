// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGWeaponInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API UBGWeaponInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


public:
	void BindWeapon(class ABGWeapon* NewWeapon);
	void UpdateCurrentAmmoText();
	void UpdateInfo();
	void ClearWidget();

protected:
	UPROPERTY()
	class ABGWeapon* Weapon;
	
	UPROPERTY()
	class UImage* WeaponImage;
	
	UPROPERTY()
	class UTextBlock* WeaponName;

	UPROPERTY()
	class UTextBlock* CurrentAmmoText;

	UPROPERTY()
	class UTextBlock* TotalAmmoText;
	
};
