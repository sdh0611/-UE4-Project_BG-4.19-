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

public:
	void AddItemRow(FBGShopItemData* NewItemData);
	void AddItemRowToSellList(FBGShopItemData* NewItemData);

public:
	virtual void BeginDestroy() override;

protected:
	//UFUNCTION(BlueprintCallable, Category = ShopWidget)
	void InitShopTab(const FName& TabName, FBGShopItemData* NewItemData);

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
	TWeakObjectPtr<ABGShop> Shop;

};
