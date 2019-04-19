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

		// ItemType�� WEAPON���� üũ.
		// ���� WEAPON�̸� Weapon Inventory���� Update�ؾ��ϹǷ�
		// �Ϲ� Inventoryâ�� �߰��ϸ� �ȵ�.
		if (EItemType::WEAPON != NewItem->GetItemType())
		{
			CurrentWeight += NewItem->GetItemWeight();
			// �ش� �������� �̹� List�� �����ϸ� ���� ����, NewItem Destroy
			// ���� ��� List�� �߰�����.
			auto Item = GetItemByName(NewItem->GetItemName());
			if ((nullptr != Item) && (Item->GetNumberOfItem() < Item->GetMaxNumberOfItem()))
			{
				//Item���� ���� �� Widget Update (BGItem Class ������ ��������Ʈ�� ȣ���Ͽ� ������Ʈ��.)
				Item->AdjustItemNumber(NewItem->GetNumberOfItem());
				NewItem->Destroy();
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("Add ItemList : %s"), *NewItem->GetItemName());
				//���� Player Inventory�� ���� item�̹Ƿ� �߰�����.
				ItemList.Add(NewItem);

				// Update InventoryWIdget
				auto PlayerController = Cast<ABGPlayerController>(OwnerPlayer->GetController());
				if (PlayerController)
				{
					auto InventoryWIdget = Cast<UBGInventoryWidget>(PlayerController->GetBGHUD()->GetInventoryWidget());
					// ���� �κ��丮�� ���� ���ο� item�� ��� Widget�� ���� �߰�����.
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
	//Item�� �����ϴ��� �˻�
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

	// ItemList�� �߰�
	AddItem(NewWeapon);

	// Player�� WeaponInventory�� ������ Index
	int32 NewWeaponIndex = -1;

	switch (NewWeapon->GetWeaponType())
	{
		case EWeaponType::PISTOL:
		{
			//�̱��� �κ�
			break;
		}
		case EWeaponType::GRENADE:
		{
			//�̱��� �κ�
			break;
		}
		default:
		{
			//�� ���Ⱑ �ϳ��� ���ٸ�
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
				//1, 2 ���� �� �ϳ��� �ֹ��Ⱑ �ִ� ���
				if (nullptr == WeaponInventory[0])
				{
					UE_LOG(LogClass, Warning, TEXT("Inventory 1-1!"));
					WeaponInventory[0] = NewWeapon;
					//���� ���⸦ ���������� �ʴٸ�
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
					//���� ���⸦ ���������� �ʴٸ�
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
				// 1, 2 �ֹ��� ������ ���� �� ���� ���
				// ���� �������� ���⸦ WeaponPickup���� �������� CurrentWeapon�� ������ ����� ��ü
				UE_LOG(LogClass, Warning, TEXT("Change Current Weapon"));
				//// ������Ʈ���� �����ϴ� ItemList���� ����
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

		////ItemList���� ����
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

