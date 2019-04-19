// Fill out your copyright notice in the Description page of Project Settings.

#include "BGShop.h"
#include "BGItem.h"
#include "BGPlayer.h"
#include "BGShopWidget.h"
#include "BGWeapon.h"
#include "BGWeaponPickup.h"
#include "BGRecovery.h"
#include "BGRecoveryPickup.h"
#include "BGGameInstance.h"
#include "BGPlayerItemStatusComponent.h"
#include "BGPlayerController.h"
#include "BGHUD.h"
#include "Components/Button.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"


// Sets default values
ABGShop::ABGShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = ShopMesh;

	// For Test
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_Shop(TEXT("StaticMesh'/Engine/EditorMeshes/ColorCalibrator/SM_ColorCalibrator.SM_ColorCalibrator'"));
	if (SM_Shop.Succeeded())
	{
		ShopMesh->SetStaticMesh(SM_Shop.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_ShopItem(TEXT("DataTable'/Game/GameData/ShopItemData.ShopItemData'"));
	if (DT_ShopItem.Succeeded())
	{
		ShopItemDataTable = DT_ShopItem.Object;
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("ShopitemDataTable not exist.."));
	}
	
	WeaponItemClass = ABGWeapon::StaticClass();
	RecoveryItemClass = ABGRecovery::StaticClass();
}

// Called when the game starts or when spawned
void ABGShop::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABGShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGShop::OnInteraction(APawn* Pawn)
{
	auto Player = Cast<ABGPlayer>(Pawn);
	//Activate shop UI
	if (Player)
	{
		auto PlayerController = Cast<ABGPlayerController>(Player->GetController());
		if (PlayerController)
		{
			auto HUD = PlayerController->GetBGHUD();
			if (HUD)
			{
				if (!HUD->IsShopOnScreen())
				{
					EnterPlayer = Player;
					HUD->GetShopWidget()->BindShopPointer(this);
					ConstructShopWidget(PlayerController);					
					HUD->DrawShopWidgetOnScreen();
				}
			}
		}

	}


}

void ABGShop::SellItemToPlayer(const FBGShopItemData& NewShopItem)
{
	if (EnterPlayer)
	{
		auto PlayerItemStatus = EnterPlayer->GetPlayerItemStatusComponent();
		if (PlayerItemStatus)
		{
			//�����ϱ⿡ ����� ���� ������ �ִ��� üũ��.
			if (NewShopItem.ItemPrice > PlayerItemStatus->GetCurrentMoney())
			{
				return;
			}

			//���ݸ�ŭ Player������ ������ ��.
			PlayerItemStatus->AddMoney(-1*NewShopItem.ItemPrice);

			switch (GetItemTypeFromString(NewShopItem.ItemType))
			{
			case EItemType::WEAPON: //Player buy weapon
			{
				//Spawn weapon
				ABGWeapon* NewWeapon = GetWorld()->SpawnActor<ABGWeapon>(WeaponItemClass);
				if (NewWeapon)
				{
					//Get weapon data from GameInstance, init weapon's data & equip.
					UBGGameInstance* GameInstance = Cast<UBGGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						NewWeapon->InitItemData(GameInstance->GetWeaponDataByName(NewShopItem.ItemName));
						NewWeapon->PickupClass = ABGWeaponPickup::StaticClass();
						EnterPlayer->EquipWeapon(NewWeapon);
					}
				}
				else
				{
					UE_LOG(LogClass, Warning, TEXT("Sell Weapon Fail!!!!"));
				}
				break;
			}
			case EItemType::RECOVERY: //Player buy recovery
			{
				ABGRecovery* NewRecovery = GetWorld()->SpawnActor<ABGRecovery>(RecoveryItemClass);
				if (NewRecovery)
				{
					UBGGameInstance* GameInstance = Cast<UBGGameInstance>(GetGameInstance());
					if (GameInstance)
					{
						NewRecovery->InitItemData(GameInstance->GetRecoveryDataByName(NewShopItem.ItemName));
						NewRecovery->PickupClass = ABGRecoveryPickup::StaticClass();
						auto ItemStatus = EnterPlayer->GetPlayerItemStatusComponent();
						if (ItemStatus)
						{
							ItemStatus->AddItem(NewRecovery);
						}

					}
				}
				break;
			}
			case EItemType::DOPING: //Player buy doping
			{

				break;
			}

			}

		}
	}

}

void ABGShop::BuyItemFromPlayer(const FBGShopItemData& NewShopItem)
{
	if (EnterPlayer)
	{
		auto PlayerItemStatus = EnterPlayer->GetPlayerItemStatusComponent();
		if (PlayerItemStatus)
		{
			// �� ���� ������ ���Ű��� 70%
			PlayerItemStatus->AddMoney((int32)(NewShopItem.ItemPrice * 0.7f));
			////�κ��丮���� Item����
			//PlayerItemStatus->RemoveItem(NewShopItem.ItemName);

			switch (GetItemTypeFromString(NewShopItem.ItemType))
			{
				case EItemType::WEAPON:
				{
					//

					break;
				}
				case EItemType::RECOVERY:
				{

					break;
				}
				case EItemType::DOPING:
				{

					break;
				}
				default:
				{
					UE_LOG(LogClass, Error, TEXT("Invalid Item Type!"));
					break;
				}
			}

		}

	}
}

void ABGShop::OnPlayerExitShop()
{
	EnterPlayer = nullptr;
}

const FBGShopItemData * const ABGShop::FindShopItemData(const FString & ItemName)
{
	TArray<FName> Names = ShopItemDataTable->GetRowNames();

	for (const auto& Name : Names)
	{
		FBGShopItemData* ShopItem = ShopItemDataTable->FindRow<FBGShopItemData>(Name, TEXT(""));
		if (ItemName == ShopItem->ItemName)
		{
			return ShopItem;
		}
	}

	return nullptr;
}

void ABGShop::ConstructShopWidget(ABGPlayerController* PlayerController)
{
	auto ShopWidget = PlayerController->GetBGHUD()->GetShopWidget();

	// ������ �ʱ�ȭ
	TArray<FName> Names = ShopItemDataTable->GetRowNames();

	for (auto NameIt = Names.CreateConstIterator(); NameIt; ++NameIt)
	{
		// Add ItemData in shop widget
		// ������ �Ǹ��ǰ� ������ �ʱ�ȭ ������ ��.
		const FBGShopItemData* ShopItem = ShopItemDataTable->FindRow<FBGShopItemData>(*NameIt, TEXT(""));
		ShopWidget->AddItemRow(ShopItem);
	}

	// �Ǹ��� �ʱ�ȭ
	auto PlayerItemStatus = EnterPlayer->GetPlayerItemStatusComponent();
	if (PlayerItemStatus)
	{
		for (int Index = 0; Index < PlayerItemStatus->GetNumberOfItemList(); ++Index)
		{
			UE_LOG(LogClass, Warning, TEXT("SellTab"));
			// 04.18 : Recovery�� ShopItemData�� ��ġ�������� �ʾƼ� ���� �Ǹ��ǿ� �ȶߴ� ������ ����.
			const FBGShopItemData* ShopItem = FindShopItemData(PlayerItemStatus->GetItemByIndex(Index)->GetItemName());
			ShopWidget->AddItemRowToSellList(ShopItem);
		}
	}


}

