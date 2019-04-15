// Fill out your copyright notice in the Description page of Project Settings.

#include "BGInventoryWidget.h"
#include "BGInventoryItemWidget.h"
#include "BGWeaponInventoryWidget.h"
#include "BGItem.h"
#include "BGPlayerController.h"
#include "BGWeapon.h"
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

	static ConstructorHelpers::FClassFinder<UBGWeaponInventoryWidget>
		UI_WeaponInventory(TEXT("WidgetBlueprint'/Game/UI/UI_UserWeaponInfo.UI_UserWeaponInfo_C'"));
	if (UI_WeaponInventory.Succeeded())
	{
		WeaponInventoryWidgetClass = UI_WeaponInventory.Class;
	}

	WeaponWidgetList.Init(nullptr, 5);

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

	UScrollBox* NewWeaponHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("WeaponHolder")));
	if (NewWeaponHolder)
	{
		WeaponHolder = NewWeaponHolder;
		//WeaponInventory 초기화 코드 넣어줄 것.
		auto PlayerController = Cast<ABGPlayerController>(GetWorld()->GetFirstPlayerController());
		for (auto& WeaponWidget : WeaponWidgetList)
		{
			auto NewWeaponWidget = CreateWidget<UBGWeaponInventoryWidget>(PlayerController, WeaponInventoryWidgetClass);
			if (NewWeaponWidget)
			{
				WeaponWidget = NewWeaponWidget;
				WeaponHolder->AddChild(WeaponWidget);
				WeaponWidget->SetPadding(5.f);
			}
		}

	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Cannot find weapon holder.."));
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
	if (ItemHolder)
	{
		UBGInventoryItemWidget* NewInventoryItem = CreateWidget<UBGInventoryItemWidget>(PlayerController, InventoryItemWidgetClass);
		if (NewInventoryItem)
		{
			UE_LOG(LogClass, Warning, TEXT("Add item to item holder.."));
			ItemHolder->AddChild(NewInventoryItem);
			NewInventoryItem->BindItemPointer(NewItem);
			NewInventoryItem->SetPadding(10.f);
			NewItem->OnItemInfoChanged.BindUObject(NewInventoryItem, &UBGInventoryItemWidget::UpdateItemInfo);
		}
		else
		{
			UE_LOG(LogClass, Error, TEXT("Cannot find item holder.."));
		}

	}

}

void UBGInventoryWidget::AddWeaponToInventoryWidget(APlayerController * PlayerController, ABGWeapon * NewWeapon)
{
	if (WeaponHolder)
	{
		UBGWeaponInventoryWidget* NewWeaponInventory = CreateWidget<UBGWeaponInventoryWidget>(PlayerController, WeaponInventoryWidgetClass);
		if (NewWeaponInventory)
		{
			UE_LOG(LogClass, Warning, TEXT("Add weapon to weapon holder.."));
			WeaponHolder->AddChild(NewWeaponInventory);
			NewWeaponInventory->BindWeapon(NewWeapon);
			NewWeaponInventory->SetPadding(5.f);
			NewWeapon->OnWeaponFire.AddUObject(NewWeaponInventory, &UBGWeaponInventoryWidget::UpdateCurrentAmmoText);
		}

	}

}

void UBGInventoryWidget::AddWeaponToInventoryWidget(ABGWeapon * NewWeapon)
{
	if (WeaponHolder)
	{
		if (NewWeapon)
		{
			auto WeaponWidget = WeaponWidgetList[NewWeapon->GetWeaponInventoryIndex()];
			WeaponWidget->BindWeapon(NewWeapon);
			NewWeapon->OnWeaponFire.AddUObject(WeaponWidget, &UBGWeaponInventoryWidget::UpdateCurrentAmmoText);
		}
	}

}

void UBGInventoryWidget::RemoveItemFromInventoryWidget()
{

}

void UBGInventoryWidget::RemoveWeaponFromInventoryWidget()
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

UBGWeaponInventoryWidget * const UBGInventoryWidget::GetWeaponInvnetoryWidget(int32 NewWeaponIndex)
{
	if (!FMath::IsWithinInclusive<int32>(NewWeaponIndex, 0, 5))
	{
		return nullptr;
	}

	return WeaponWidgetList[NewWeaponIndex];
}

void UBGInventoryWidget::ConstructWidget()
{
	UScrollBox* NewItemHolder = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemHolder")));
	if (NewItemHolder)
	{
		ItemHolder = NewItemHolder;
	}


}
