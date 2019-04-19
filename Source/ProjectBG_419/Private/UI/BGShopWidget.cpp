// Fill out your copyright notice in the Description page of Project Settings.

#include "BGShopWidget.h"
#include "BGShopItemRowWidget.h"
#include "BGShop.h"
#include "BGItem.h"
#include "BGPlayerController.h"
#include "BGPlayerItemStatusComponent.h"
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

void UBGShopWidget::BindPlayerItemStatus(UBGPlayerItemStatusComponent * NewPlayerItemStatus)
{
	PlayerItemStatus = NewPlayerItemStatus;
}

void UBGShopWidget::AddItemRow(const FBGShopItemData * NewItemData)
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
			InitShopTab(TEXT("WeaponItemHolder"), NewItemData, false);
			break;
		}
		case EItemType::RECOVERY:
		{
			InitShopTab(TEXT("RecoveryItemHolder"), NewItemData, false);
			break;
		}
		case EItemType::DOPING:
		{
			InitShopTab(TEXT("DopingItemHolder"), NewItemData, false);
			break;
		}
		default:
		{
			UE_LOG(LogClass, Error, TEXT("Invalid item type in UBGShopWidget::AddItemRow"));
			break;
		}
	}


}

void UBGShopWidget::AddItemRowToSellList(const FBGShopItemData * NewItemData)
{
	if (nullptr == NewItemData)
	{
		UE_LOG(LogClass, Error, TEXT("Invalid item data in UBGShopWidget::AddItemRowToSellList"));
		return;
	}

	//Selling Tab으로 초기화하게끔 수정하자.
	EItemType ItemType = GetItemTypeFromString(NewItemData->ItemType);

	switch (ItemType)
	{
		case EItemType::WEAPON:
		{
			UE_LOG(LogClass, Error, TEXT("Add WEAPON"));
			InitShopTab(TEXT("WeaponItemHolder_Sell"), NewItemData, true);
			break;
		}
		case EItemType::RECOVERY:
		{
			UE_LOG(LogClass, Error, TEXT("Add RECOVERY"));
			InitShopTab(TEXT("RecoveryItemHolder_Sell"), NewItemData, true);
			break;
		}
		case EItemType::DOPING:
		{
			UE_LOG(LogClass, Error, TEXT("Add DOPING"));
			InitShopTab(TEXT("DopingItemHolder_Sell"), NewItemData, true);
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
	// 구매탭 Clear
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

		// 판매탭 Clear
		auto SellWidgetSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("SellingWidgetSwitcher")));
		if (SellWidgetSwitcher)
		{
			int32 Num = SellWidgetSwitcher->GetNumWidgets();
			for (int32 WidgetIndex = 0; WidgetIndex < Num; ++WidgetIndex)
			{
				auto ItemHolder = Cast<UScrollBox>(SellWidgetSwitcher->GetWidgetAtIndex(WidgetIndex));
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


void UBGShopWidget::InitShopTab(const FName& TabName, const FBGShopItemData* NewItemData, bool bInitSellTab)
{
	UScrollBox* ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TabName));
	if (ScrollBox)
	{
		UBGShopItemRowWidget* ItemRow = CreateWidget<UBGShopItemRowWidget>(GetOwningPlayer(), ItemRowWidgetClass);
		if (ItemRow)
		{
			ItemRow->SetItemData(NewItemData);
			ItemRow->SetIsSellTab(bInitSellTab);

			// SellTab 초기화일 경우와 아닌 경우 해당 ItemRow의 기능이 달라짐.
			if (bInitSellTab)
			{
				ItemRow->OnItemRowBtnClick.BindLambda([this](const FBGShopItemData& NewItemData)->void
				{
					Shop->BuyItemFromPlayer(NewItemData);
				});
			}
			else
			{
				ItemRow->OnItemRowBtnClick.BindLambda([this](const FBGShopItemData& NewItemData)->void
				{
					Shop->SellItemToPlayer(NewItemData);
				});
			}

			ScrollBox->AddChild(ItemRow);
			ItemRow->SetPadding(5.f);
		}
		else
		{
			UE_LOG(LogClass, Error, TEXT("ShopItemdata invalid in UBGShopWidget::AddItemRow"));
		}
	}

}


