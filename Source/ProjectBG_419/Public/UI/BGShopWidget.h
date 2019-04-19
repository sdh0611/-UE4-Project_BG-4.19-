// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "Blueprint/UserWidget.h"
#include "BGShopWidget.generated.h"

/**
 * 
 */


UCLASS()
class PROJECTBG_419_API UBGShopWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:
	UBGShopWidget(const FObjectInitializer & ObjectInitializer);

protected:
	//Called when add to viewport.
	virtual void NativeConstruct() override;

public:
	void BindShopPointer(class ABGShop* NewShop);
	void BindPlayerItemStatus(class UBGPlayerItemStatusComponent* NewPlayerItemStatus);

public:
	void AddItemRow(const FBGShopItemData* NewItemData);
	void AddItemRowToSellList(const FBGShopItemData* NewItemData);

public:
	virtual void BeginDestroy() override;

protected:
	//UFUNCTION(BlueprintCallable, Category = ShopWidget)
	void InitShopTab(const FName& TabName, const FBGShopItemData* NewItemData, bool bInitSellTab);

private:
	UFUNCTION()
	void OnExitButtonClicked();

//public:
//	FOnBuyItem OnBuyItem;

protected:
	//TSubclassOf<UUserWidget> ItemRowWidgetClass;
	TSubclassOf<class UBGShopItemRowWidget> ItemRowWidgetClass;

protected:
	UPROPERTY()
	class UButton* ShopExitButton;

	UPROPERTY()
	TWeakObjectPtr<class ABGShop> Shop;

	UPROPERTY()
	TWeakObjectPtr<class UBGPlayerItemStatusComponent> PlayerItemStatus;

};
