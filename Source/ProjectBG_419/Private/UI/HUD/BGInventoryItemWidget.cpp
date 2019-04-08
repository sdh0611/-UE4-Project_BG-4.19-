// Fill out your copyright notice in the Description page of Project Settings.

#include "BGInventoryItemWidget.h"
#include "BGPlayerItemStatusComponent.h"
#include "BGItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"


void UBGInventoryItemWidget::NativeConstruct()
{
	UE_LOG(LogClass, Error, TEXT("Construct Inventory Item."));

	Super::NativeConstruct();

	ConstructWidget();

}

void UBGInventoryItemWidget::ConstructWidget()
{
	//UImage* NewItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	//if (NewItemImage)
	//{
	//	ItemImage = NewItemImage;
	//}
	//else
	//{
	//	UE_LOG(LogClass, Error, TEXT("ItemImage is null."));
	//}

	//UTextBlock* NewItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	//if (NewItemName)
	//{
	//	ItemName = NewItemName;
	//}
	//else
	//{
	//	UE_LOG(LogClass, Error, TEXT("ItemName is null."));
	//}

	//UTextBlock* NewNumberOfItem = Cast<UTextBlock>(GetWidgetFromName(TEXT("NumberOfItem")));
	//if (NewNumberOfItem)
	//{
	//	NumberOfItem = NewNumberOfItem;
	//}
	//else
	//{
	//	UE_LOG(LogClass, Error, TEXT("NumberOfItem is null."));
	//}


}

void UBGInventoryItemWidget::BindItemPointer(ABGItem * NewItem)
{
	if (nullptr == NewItem)
	{
		UE_LOG(LogClass, Error, TEXT("NewItem is null."));
		return;
	}
	
	ItemPointer = NewItem;
	SetUIProperties();
	UE_LOG(LogClass, Error, TEXT("Bind item pointer."));
	//ItemName->SetText(FText::FromName(ItemPointer->GetItemName()));
	//NumberOfItem->SetText(FText::FromString(FString::FromInt(ItemPointer->GetNumberOfItem())));
}
