// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
//#include "BGType.h"
#include "Blueprint/UserWidget.h"
#include "BGShopItemRowWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnBuyItem, const FBGShopItemData&);

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API UBGShopItemRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Called when add to viewport.
	virtual void NativeConstruct() override;

public:
	void SetItemData(FBGShopItemData* NewItemData);
	const FBGShopItemData& GetItemData() const;

private:
	UFUNCTION()
	void OnBuyButtonClicked();

public:
	FOnBuyItem OnBuyItem;

private:
	FBGShopItemData ItemData;

private:
	UPROPERTY()
	class UImage* ItemImage;

	UPROPERTY()
	class UTextBlock* ItemName;

	UPROPERTY()
	class UTextBlock* ItemPrice;

	UPROPERTY()
	class UButton* BuyButton;

};
