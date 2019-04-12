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

//반환할 값은 Player가 들고있어야 할 무기의 포인터.
ABGWeapon* UBGPlayerItemStatusComponent::EquipWeapon(ABGWeapon * NewWeapon)
{
	ABGWeapon* Weapon = nullptr;

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
			}
			else if (nullptr == WeaponInventory[0] || nullptr == WeaponInventory[1])
			{
				//1, 2 슬롯 중 하나만 주무기가 있는 경우
				if (nullptr == WeaponInventory[0])
				{
					//UE_LOG(LogClass, Warning, TEXT("Inventory 1-1!"));
					WeaponInventory[0] = NewWeapon;
					//현재 무기를 장착중이지 않다면
					if (nullptr == OwnerPlayer->GetCurrentWeapon())
					{
						SetCurrentWeaponIndex(0);
						Weapon = NewWeapon;
					}

				}
				else if (nullptr == WeaponInventory[1])
				{
					//UE_LOG(LogClass, Warning, TEXT("Inventory 1-2!"));
					WeaponInventory[1] = NewWeapon;
					//현재 무기를 장착중이지 않다면
					if (nullptr == OwnerPlayer->GetCurrentWeapon())
					{
						SetCurrentWeaponIndex(1);
						Weapon = NewWeapon;
					}
					
				}
			}
			else
			{
				//1, 2 주무기 슬롯이 전부 차 있을 경우
				//현재 착용중인 무기를 WeaponPickup으로 돌려놓고 CurrentWeapon을 습득한 무기로 교체
				//UE_LOG(LogClass, Warning, TEXT("Change Current Weapon"));
				Weapon = NewWeapon;
				WeaponInventory[CurrentWeaponIndex] = NewWeapon;
			}

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

ABGWeapon* const UBGPlayerItemStatusComponent::GetWeapon(int32 NewWeaponIndex)
{
	//CurrentWeaponIndex = NewWeaponIndex;
	if (!FMath::IsWithinInclusive<int32>(NewWeaponIndex, 0, WeaponInventory.Num() - 1))
	{
		return nullptr;
	}

	return WeaponInventory[NewWeaponIndex];
}

