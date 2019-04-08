// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGInventoryItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API UBGInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void ConstructWidget();
	void BindItemPointer(class ABGItem* NewItem);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = InventoryItem)
	void SetUIProperties();

protected:
	


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=InventoryItem)
	class ABGItem* ItemPointer;

protected:
	//UPROPERTY()
	//class UImage* ItemImage;

	//UPROPERTY()
	//class UTextBlock* ItemName;

	//UPROPERTY()
	//class UTextBlock* NumberOfItem;
	//


};
