// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API UBGInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBGInventoryWidget(const FObjectInitializer & ObjectInitializer);

public:
	virtual void NativeConstruct() override;
	virtual void RemoveFromParent() override;
	   
public:
	void AddItemToInventoryWidget(class APlayerController* PlayerController, class ABGItem* NewItem);
	void AddWeaponToInventoryWidget(class APlayerController* PlayerController, class ABGWeapon* NewWeapon);
	void AddWeaponToInventoryWidget(class ABGWeapon* NewWeapon);
	void RemoveItemFromInventoryWidget();
	void RemoveWeaponFromInventoryWidget();

public:
	void SetIsWidgetOnScreen(bool NewState);

public:
	bool IsWidgetOnScreeen() const;
	class UBGWeaponInventoryWidget* const GetWeaponInvnetoryWidget(int32 NewWeaponIndex);

protected:
	void ConstructWidget();
	
protected:
	bool bIsWidgetOnScreen;
	TSubclassOf<class UBGInventoryItemWidget> InventoryItemWidgetClass;
	TSubclassOf<class UBGWeaponInventoryWidget> WeaponInventoryWidgetClass;

protected:
	UPROPERTY()
	TArray<class UBGWeaponInventoryWidget*> WeaponWidgetList;

protected:
	UPROPERTY()
	class UScrollBox* ItemHolder;

	UPROPERTY()
	class UScrollBox* WeaponHolder;

};
