// Fill out your copyright notice in the Description page of Project Settings.

#include "BGInventoryWidget.h"
#include "BGInventoryItemWidget.h"
#include "BGItem.h"
#include "BGPlayerController.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "ConstructorHelpers.h"


UBGInventoryWidget::UBGInventoryWidget(const FObjectInitializer & ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UBGInventoryItemWidget>
		UI_InventoryItem(TEXT("WidgetBlueprint'/Game/UI/UI_InventoryItem.UI_InventoryItem_C'"));
	if (UI_InventoryItem.Succeeded())
	{
		InventoryItemWidgetClass = UI_InventoryItem.Class;
	}

	bIsWidgetOnScreen = false;
	ItemHolder = nullptr;
}

void UBGInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UScrollBox* NewItemHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemHolder")));
	if (NewItemHolder)
	{
		ItemHolder = NewItemHolder;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Cannot find item holder.."));
	}

	SetIsWidgetOnScreen(true);

	UE_LOG(LogClass, Warning, TEXT("Draw Inventory"));
}

void UBGInventoryWidget::RemoveFromParent()
{
	Super::RemoveFromParent();

	SetIsWidgetOnScreen(false);

	UE_LOG(LogClass, Warning, TEXT("Remove Inventory"));
}

void UBGInventoryWidget::AddItemToInventoryWidget(class APlayerController* PlayerController, ABGItem * NewItem)
{
	//auto PlayerController = GetOwningPlayer();
	if (ItemHolder)
	{
		//NewInventoryItem->AddToViewport();
		UBGInventoryItemWidget* NewInventoryItem = CreateWidget<UBGInventoryItemWidget>(PlayerController, InventoryItemWidgetClass);
		if (NewInventoryItem)
		{
			UE_LOG(LogClass, Warning, TEXT("Add item to item holder.."));
			ItemHolder->AddChild(NewInventoryItem);
			NewInventoryItem->BindItemPointer(NewItem);
			NewInventoryItem->SetPadding(10.f);
		}
		else
		{
			UE_LOG(LogClass, Error, TEXT("Cannot find item holder.."));
		}

	}

}

void UBGInventoryWidget::RemoveItemFromInventoryWidget()
{
}

void UBGInventoryWidget::SetIsWidgetOnScreen(bool NewState)
{
	bIsWidgetOnScreen = NewState;
}

bool UBGInventoryWidget::IsWidgetOnScreeen() const
{
	return bIsWidgetOnScreen;
}

void UBGInventoryWidget::ConstructWidget()
{
	UScrollBox* NewItemHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemHolder")));
	if (NewItemHolder)
	{
		ItemHolder = NewItemHolder;
	}


}
