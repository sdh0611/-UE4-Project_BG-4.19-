// Fill out your copyright notice in the Description page of Project Settings.

#include "BGPlayerItemStatusComponent.h"
#include "BGItem.h"
#include "BGWeapon.h"
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
	OwnerPlayer = nullptr;
	WeaponInventory.Init(nullptr, 5);
	TotalWeight = 0;
	CurrentWeight = 0;
	CurrentMoney = 0;
	CurrentWeaponIndex = 0;
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
		UE_LOG(LogClass, Warning, TEXT("Add Item : %s"), *NewItem->GetItemName());

		// ItemType이 WEAPON인지 체크.
		// 만약 WEAPON이면 Weapon Inventory에서 Update해야하므로
		// 일반 Inventory창에 추가하면 안됨.
		if (EItemType::WEAPON != NewItem->GetItemType())
		{
			CurrentWeight += NewItem->GetItemWeight();
			// 해당 아이템이 이미 List에 존재하면 개수 증가, NewItem Destroy
			// 없는 경우 List에 추가해줌.
			auto Item = GetItemByName(NewItem->GetItemName());
			if ((nullptr != Item) && (Item->GetNumberOfItem() < Item->GetMaxNumberOfItem()))
			{
				//Item개수 증가 및 Widget Update (BGItem Class 내부의 델리게이트를 호출하여 업데이트함.)
				Item->AdjustItemNumber(NewItem->GetNumberOfItem());
				NewItem->Destroy();
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("Add ItemList : %s"), *NewItem->GetItemName());
				//기존 Player Inventory에 없던 item이므로 추가해줌.
				ItemList.Add(NewItem);

				// Update InventoryWIdget
				auto PlayerController = Cast<ABGPlayerController>(OwnerPlayer->GetController());
				if (PlayerController)
				{
					auto InventoryWIdget = Cast<UBGInventoryWidget>(PlayerController->GetBGHUD()->GetInventoryWidget());
					// 기존 인벤토리에 없는 새로운 item일 경우 Widget에 새로 추가해줌.
					if (InventoryWIdget && (nullptr == Item))
					{
						InventoryWIdget->AddItemToInventoryWidget(PlayerController, NewItem);
					}

				}

			}

		}
		else
		{
			ItemList.Add(NewItem);
		}

	}
}

void UBGPlayerItemStatusComponent::RemoveItem(ABGItem * NewItem, bool bDestroy)
{
	if (ItemList.Contains(NewItem))
	{
		ItemList.RemoveSingle(NewItem);
		if (bDestroy)
		{
			NewItem->Destroy();
		}
	}

}

void UBGPlayerItemStatusComponent::RemoveItem(const FString & ItemName, bool bDestroy)
{
	//Item이 존재하는지 검사
	auto Item = GetItemByName(ItemName);
	if (Item)
	{
		RemoveItem(Item);

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

void UBGPlayerItemStatusComponent::SetCurrentWeaponIndex(int32 NewWeaponIndex)
{
	if (!FMath::IsWithinInclusive<int32>(NewWeaponIndex, 0, WeaponInventory.Num() - 1))
	{
		return;
	}

	CurrentWeaponIndex = NewWeaponIndex;

}

void UBGPlayerItemStatusComponent::AddMoney(int32 NewMoney)
{
	CurrentMoney = (CurrentMoney + NewMoney) >= 0 ? CurrentMoney + NewMoney : 0;
	OnMoneyChanged.Execute();
}

ABGWeapon* UBGPlayerItemStatusComponent::EquipWeapon(ABGWeapon * NewWeapon)
{
	ABGWeapon* Weapon = nullptr;

	// ItemList에 추가
	AddItem(NewWeapon);

	// Player의 WeaponInventory에 배정될 Index
	int32 NewWeaponIndex = -1;

	switch (NewWeapon->GetWeaponType())
	{
		case EWeaponType::PISTOL:
		{
			//미구현 부분
			break;
		}
		case EWeaponType::GRENADE:
		{
			//미구현 부분
			break;
		}
		default:
		{
			//주 무기가 하나도 없다면
			if ((nullptr == WeaponInventory[0]) && (nullptr == WeaponInventory[1]))
			{
				UE_LOG(LogClass, Warning, TEXT("Inventory 1!"));
				SetCurrentWeaponIndex(0);
				WeaponInventory[CurrentWeaponIndex] = NewWeapon;
				Weapon = NewWeapon;
				NewWeaponIndex = 0;

			}
			else if (nullptr == WeaponInventory[0] || nullptr == WeaponInventory[1])
			{
				//1, 2 슬롯 중 하나만 주무기가 있는 경우
				if (nullptr == WeaponInventory[0])
				{
					UE_LOG(LogClass, Warning, TEXT("Inventory 1-1!"));
					WeaponInventory[0] = NewWeapon;
					//현재 무기를 장착중이지 않다면
					if (nullptr == OwnerPlayer->GetCurrentWeapon())
					{
						SetCurrentWeaponIndex(0);
						Weapon = NewWeapon;
					}
					NewWeaponIndex = 0;

				}
				else if (nullptr == WeaponInventory[1])
				{
					UE_LOG(LogClass, Warning, TEXT("Inventory 1-2!"));
					WeaponInventory[1] = NewWeapon;
					//현재 무기를 장착중이지 않다면
					if (nullptr == OwnerPlayer->GetCurrentWeapon())
					{
						SetCurrentWeaponIndex(1);
						Weapon = NewWeapon;
					}
					NewWeaponIndex = 1;

				}
			}
			else
			{
				// 1, 2 주무기 슬롯이 전부 차 있을 경우
				// 현재 착용중인 무기를 WeaponPickup으로 돌려놓고 CurrentWeapon을 습득한 무기로 교체
				UE_LOG(LogClass, Warning, TEXT("Change Current Weapon"));
				//// 컴포넌트에서 관리하는 ItemList에서 제거
				//RemoveItem(Weapon, false);
				Weapon = NewWeapon;
				WeaponInventory[CurrentWeaponIndex] = NewWeapon;
				NewWeaponIndex = CurrentWeaponIndex;
			}
			NewWeapon->SetWeaponInventoryIndex(NewWeaponIndex);
			NewWeapon->SetIsEquippedByPlayer(true);

			break;
		}
	}

	return Weapon;;
}


void UBGPlayerItemStatusComponent::RemoveWeapon(ABGWeapon * NewWeapon, bool bDestroy)
{
	// If Weapon valid pointer
	if (NewWeapon)
	{
		// If Weapon in WeaponInventory
		if (WeaponInventory.Contains(NewWeapon))
		{
			int32 Index = WeaponInventory.Find(NewWeapon);
			WeaponInventory[Index] = nullptr;
			UE_LOG(LogClass, Warning, TEXT("Remove weapon in %d."), Index);
		}

		NewWeapon->SetIsEquippedByPlayer(false);

		////ItemList에서 제거
		RemoveItem(NewWeapon, bDestroy);

		if (bDestroy)
		{
			NewWeapon->Destroy();
		}
		else
		{
			NewWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			NewWeapon->SetActorEnableCollision(ECollisionEnabled::NoCollision);
			NewWeapon->SetActorHiddenInGame(true);
		}

	}
}

int32 UBGPlayerItemStatusComponent::GetCurrentMoney() const
{
	return CurrentMoney;
}

int32 UBGPlayerItemStatusComponent::GetCurrentWeaponIndex() const
{
	return CurrentWeaponIndex;
}

int32 UBGPlayerItemStatusComponent::GetNumberOfItemList() const
{
	return ItemList.Num();
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

ABGItem * const UBGPlayerItemStatusComponent::GetItemByIndex(int32 ItemIndex)
{
	if (!FMath::IsWithinInclusive<int32>(ItemIndex, 0, ItemList.Num()))
	{
		return nullptr;
	}

	return ItemList[ItemIndex];
}

ABGWeapon* const UBGPlayerItemStatusComponent::GetWeapon(int32 NewWeaponIndex)
{
	//CurrentWeaponIndex = NewWeaponIndex;
	if (!FMath::IsWithinInclusive<int32>(NewWeaponIndex, 0, WeaponInventory.Num() - 1))
	{
		return nullptr;
	}

	return WeaponInventory[NewWeaponIndex];
}

