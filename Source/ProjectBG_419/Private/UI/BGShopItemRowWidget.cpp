// Fill out your copyright notice in the Description page of Project Settings.

#include "BGShopItemRowWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UBGShopItemRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogClass, Warning, TEXT("NativeConstruct ShopRowWidget"));
	UImage* NewImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	if (NewImage)
	{
		ItemImage = NewImage;
		//ItemImage->SetBrushFromTexture();
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("NewImage invalid in UBGShopItemRowWidget::SetItemData"));
	}

	UTextBlock* NewNameTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	if (NewNameTextBlock)
	{
		ItemName = NewNameTextBlock;
		ItemName->SetText(FText::FromString(ItemData.ItemName));
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("NewName invalid in UBGShopItemRowWidget::SetItemData"));
	}

	UTextBlock* NewPriceTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemPrice")));
	if (NewPriceTextBlock)
	{
		ItemName = NewPriceTextBlock;
		ItemPrice->SetText(FText::FromString(FString::FromInt(ItemData.ItemPrice).Append(TEXT(" $"))));
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("NewPrice invalid in UBGShopItemRowWidget::SetItemData"));
	}

	UButton* NewBuyButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Buy")));
	if (NewBuyButton)
	{
		BuyButton = NewBuyButton;
		BuyButton->OnClicked.AddDynamic(this, &UBGShopItemRowWidget::OnBuyButtonClicked);
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("NewBuyButton invalid in UBGShopItemRowWidget"));
	}


}

void UBGShopItemRowWidget::SetItemData(FBGShopItemData * NewItemData)
{
	if (nullptr == NewItemData)
	{
		UE_LOG(LogClass, Error, TEXT("ItemData invalid in UBGShopItemRowWidget::SetItemData"));
		return;
	}

	ItemData = *NewItemData;

}

const FBGShopItemData& UBGShopItemRowWidget::GetItemData() const
{
	return ItemData;
}

void UBGShopItemRowWidget::OnBuyButtonClicked()
{
	OnBuyItem.Execute(ItemData);
}
