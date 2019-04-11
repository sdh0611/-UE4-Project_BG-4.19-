// Fill out your copyright notice in the Description page of Project Settings.

#include "BGPlayerItemStatusComponent.h"
#include "BGItem.h"
#include "BGInventoryWidget.h"
#include "BGPlayer.h"
#include "BGPlayerController.h"
#include "BGHUD.h"

// Sets default values for this component's properties
UBGPlayerItemStatusComponent::UBGPlayerItemStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//Init properties
	TotalWeight = 0;
	CurrentWeight = 0;
	CurrentMoney = 0;
	// ...
}


// Called when the game starts
void UBGPlayerItemStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Set Owner
	auto Owner = Cast<ABGPlayer>(GetOwner());
	if (nullptr == Owner)
	{
		UE_LOG(LogClass, Error, TEXT("Owner invalid in ItemStatusComponent.."));
	}
	OwnerPlayer = Owner;
}


// Called every frame
void UBGPlayerItemStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBGPlayerItemStatusComponent::AddItem(ABGItem * NewItem)
{
	if (NewItem)
	{
		CurrentWeight += NewItem->GetItemWeight();
		// 해당 아이템이 이미 List에 존재하면 개수 증가,
		// 없는 경우 List에 추가해줌.
		auto Item = GetItemByName(NewItem->GetItemName());
		if (Item)
		{
			Item->IncreaseItemNumber(NewItem->GetNumberOfItem());
		}
		else
		{
			ItemList.Add(NewItem);
		}

		// Update InventoryWIdget
		auto PlayerController = Cast<ABGPlayerController>(OwnerPlayer->GetController());
		if (PlayerController)
		{
			auto InventoryWIdget = Cast<UBGInventoryWidget>(PlayerController->GetBGHUD()->GetInventoryWidget());
			if (InventoryWIdget && (nullptr == Item))
			{
				InventoryWIdget->AddItemToInventoryWidget(PlayerController, NewItem);
			}

		}
		
	}
}

void UBGPlayerItemStatusComponent::SetMoney(int32 NewMoney)
{
	if (NewMoney >= 0)
	{
		CurrentMoney = NewMoney;
		OnMoneyChanged.Execute();
	}
}

void UBGPlayerItemStatusComponent::AddMoney(int32 NewMoney)
{
	CurrentMoney = (CurrentMoney + NewMoney) >= 0 ? CurrentMoney + NewMoney : 0;
	OnMoneyChanged.Execute();
}

int32 UBGPlayerItemStatusComponent::GetCurrentMoney() const
{
	return CurrentMoney;
}

ABGItem * const UBGPlayerItemStatusComponent::GetItemByName(const FString & ItemName)
{
	for (const auto& Item : ItemList)
	{
		if (Item->GetItemName().Equals(ItemName))
		{
			return Item;
		}
	}

	return nullptr;
}

