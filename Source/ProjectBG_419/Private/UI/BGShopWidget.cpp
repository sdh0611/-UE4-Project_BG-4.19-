// Fill out your copyright notice in the Description page of Project Settings.

#include "BGShopWidget.h"
#include "BGShopItemRowWidget.h"
#include "BGShop.h"
#include "BGItem.h"
#include "BGPlayerController.h"
#include "BGHUD.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "ConstructorHelpers.h"

UBGShopWidget::UBGShopWidget(const FObjectInitializer & ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UBGShopItemRowWidget>
		UI_ItemRow(TEXT("WidgetBlueprint'/Game/UI/UI_ItemRow.UI_ItemRow_C'"));
	if (UI_ItemRow.Succeeded())
	{
		ItemRowWidgetClass = UI_ItemRow.Class;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("ItemRowUIClass not exist."));
	}
	
}

void UBGShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ShopExitButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Exit")));
	if (nullptr == ShopExitButton)
	{
		UE_LOG(LogClass, Error, TEXT("ShopExitButton not exist."));
	}
	else
	{
		ShopExitButton->OnClicked.AddDynamic(this, &UBGShopWidget::OnExitButtonClicked);
	}
	
}

void UBGShopWidget::BindShopPointer(ABGShop * NewShop)
{
	Shop = NewShop;

}

void UBGShopWidget::AddItemRow(FBGShopItemData * NewItemData)
{
	if (nullptr == NewItemData)
	{
		UE_LOG(LogClass, Error, TEXT("Invalid item data in UBGShopWidget::AddItemRow"));
		return;
	}

	EItemType ItemType = GetItemTypeFromString(NewItemData->ItemType);

	switch (ItemType)
	{
	case EItemType::WEAPON:
	{
		InitShopTab(TEXT("WeaponItemHolder"), NewItemData);
		break;
	}
	case EItemType::RECOVERY:
	{
		InitShopTab(TEXT("RecoveryItemHolder"), NewItemData);
		break;
	}
	case EItemType::DOPING:
	{
		InitShopTab(TEXT("DopingItemHolder"), NewItemData);
		break;
	}
	default:
	{
		UE_LOG(LogClass, Error, TEXT("Invalid item type in UBGShopWidget::AddItemRow"));
		break;
	}
	}


}

void UBGShopWidget::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogClass, Warning, TEXT("ShopWidget BeginDestroy!"));
}

void UBGShopWidget::OnExitButtonClicked()
{
	auto PlayerController = Cast<ABGPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		auto WidgetSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("ShopWidgetSwitcher")));
		if (WidgetSwitcher)
		{
			int32 Num = WidgetSwitcher->GetNumWidgets();
			for (int32 WidgetIndex = 0; WidgetIndex < Num; ++WidgetIndex)
			{
				auto ItemHolder = Cast<UScrollBox>(WidgetSwitcher->GetWidgetAtIndex(WidgetIndex));
				if (ItemHolder)
				{
					ItemHolder->ClearChildren();
				}
			}

		}

		if (Shop.IsValid())
		{
			Shop->OnPlayerExitShop();
		}
		PlayerController->GetBGHUD()->RemoveShopWidgetOnScreen();
	}
	//GetOwningPlayer()->bShowMouseCursor = false;
	//GetOwningPlayer()->SetInputMode(FInputModeGameOnly());

	//RemoveFromParent();
}

//void UBGShopWidget::OnBuyButtonClicked()
//{
//	//OnBuyItem.Execute();
//	
//}

void UBGShopWidget::InitShopTab(const FName& TabName, FBGShopItemData* NewItemData)
{
	UScrollBox* ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TabName));
	if (ScrollBox)
	{
		UBGShopItemRowWidget* ItemRow = CreateWidget<UBGShopItemRowWidget>(GetOwningPlayer(), ItemRowWidgetClass);
		if (ItemRow)
		{
			ItemRow->SetItemData(NewItemData);

			ScrollBox->AddChild(ItemRow);
			ItemRow->SetPadding(5.f);
			ItemRow->OnBuyItem.BindLambda([this](const FBGShopItemData& NewItemData)->void
			{
				Shop->SoldToPlayer(NewItemData);
			});
		}
		else
		{
			UE_LOG(LogClass, Error, TEXT("ShopItemdata invalid in UBGShopWidget::AddItemRow"));
		}
	}

}


