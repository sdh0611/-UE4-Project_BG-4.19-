// Fill out your copyright notice in the Description page of Project Settings.

#include "BGShop.h"
#include "BGItem.h"
#include "BGPlayer.h"
#include "BGShopWidget.h"
#include "BGWeapon.h"
#include "BGWeaponPickup.h"
#include "BGRecovery.h"
#include "BGGameInstance.h"
#include "BGPlayerItemStatusComponent.h"
#include "BGPlayerController.h"
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


	static ConstructorHelpers::FClassFinder<UBGShopWidget>
		UI_Shop(TEXT("WidgetBlueprint'/Game/UI/UI_ShopWidget.UI_ShopWidget_C'"));
	if (UI_Shop.Succeeded())
	{
		ShopWidgetClass = UI_Shop.Class;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Shop ui not exist.."));
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
	
	//ShopWidgetClass = CreateWidget(, ShopWidgetClass);
	
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
		if (ShopWidgetClass)
		{
			auto PlayerController = Cast<APlayerController>(Player->GetController());
			if (PlayerController)
			{
				ConstructShopWidget(PlayerController);
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeUIOnly());
				EnterPlayer = Player;
			}
		}
		else
		{
			UE_LOG(LogClass, Error, TEXT("Shop ui invalid."));
		}
	}


}

void ABGShop::SoldToPlayer(const FBGShopItemData& NewShopItem)
{
	if (EnterPlayer)
	{
		auto PlayerItemStatus = EnterPlayer->GetPlayerItemStatusComponent();
		if (PlayerItemStatus)
		{
			//구매하기에 충분한 돈을 가지고 있는지 체크함.
			if (NewShopItem.ItemPrice > PlayerItemStatus->GetCurrentMoney())
			{
				return;
			}

			//가격만큼 Player소유의 돈에서 뺌.
			PlayerItemStatus->AddMoney(-1*NewShopItem.ItemPrice);

			//일단 Weapon만 테스트
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
				break;
			}
			case EItemType::RECOVERY: //Player buy recovery
			{

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

void ABGShop::PurchaseFromPlayer()
{
	if (EnterPlayer)
	{
		auto PlayerItemStatus = EnterPlayer->GetPlayerItemStatusComponent();
		if (PlayerItemStatus)
		{
			//이부분을 이제 각각의 Item가격에 맞게 값을 조정하고, Player에게 스폰시켜주도록 해보자.
			PlayerItemStatus->AddMoney(500);
			
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

void ABGShop::ConstructShopWidget(APlayerController* PlayerController)
{
	//if (nullptr == ShopWidget)
	//{
	ShopWidget = CreateWidget<UBGShopWidget>(PlayerController, ShopWidgetClass);

	TArray<FName> Names = ShopItemDataTable->GetRowNames();

	for (auto NameIt = Names.CreateConstIterator(); NameIt; ++NameIt)
	{
		//Add ItemData in shop widget
		FBGShopItemData* ShopItem = ShopItemDataTable->FindRow<FBGShopItemData>(*NameIt, TEXT(""));
		ShopWidget->AddItemRow(ShopItem);
	}

	ShopWidget->AddToViewport();
	ShopWidget->BindShopPointer(this);

	//}
	//else
	//{
	//	ShopWidget->AddToViewport();
	//}
}

