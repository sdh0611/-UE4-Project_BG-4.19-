// Fill out your copyright notice in the Description page of Project Settings.

#include "BGWeaponPickup.h"
#include "BGWeapon.h"
#include "BGPlayer.h"
#include "BGGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"

ABGWeaponPickup::ABGWeaponPickup()
{
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>
	//	SM_WEAPON(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SM_AR4.SM_AR4'"));

	//if (SM_WEAPON.Succeeded())
	//{
	//	Mesh->SetStaticMesh(SM_WEAPON.Object);
	//}

	ItemClass = ABGWeapon::StaticClass();
	WeaponData = nullptr;
	Weapon = nullptr;
}

void ABGWeaponPickup::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Init WeaponData from Gameinstance
	auto GameInstance = Cast<UBGGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		int32 Num = FMath::RandRange(1.f, (float)(GameInstance->GetSkeletalMeshTableSize()+1));
		WeaponData = GameInstance->GetWeaponDataByRowNumber(Num);
		if (WeaponData)
		{
			auto StaticMesh = GameInstance->GetStaticMesh(WeaponData->ItemName);
			if (StaticMesh)
			{
				UE_LOG(LogClass, Warning, TEXT("Init PickupMesh!"));
				Mesh->SetStaticMesh(StaticMesh);
			}
		}
	}

}

void ABGWeaponPickup::OnInteraction(APawn* Pawn)
{
	auto Player = Cast<ABGPlayer>(Pawn);
	if (Player)
	{
		if (Weapon) // If weapon was dropped at once.
		{
			//Active Weapon & Attach to owner pawn.
			Weapon->SetActorEnableCollision(ECollisionEnabled::QueryOnly);
			Weapon->SetActorHiddenInGame(false);
		}
		else
		{
			Weapon = GetWorld()->SpawnActor<ABGWeapon>(ItemClass);
			Weapon->InitItemData(WeaponData);
		}
		Player->EquipWeapon(Weapon);
		//Weapon->SetWeaponOwner(Player);
		Weapon->PickupClass = StaticClass();

		Super::OnInteraction(Pawn);
	}
	
	//if (Player)
	//{
	//	auto Weapon = GetWorld()->SpawnActor<ABGWeapon>(WeaponClass);
	//	Weapon->InitWeaponStat(WeaponData);

	//	Player->EquipWeapon(Weapon);

	//	Weapon->SetWeaponOwner(Player);
	//	Weapon->PickupClass = StaticClass();
	//}

}

void ABGWeaponPickup::SetWeapon(ABGWeapon * NewWeapon)
{
	if (NewWeapon)
	{
		Weapon = NewWeapon;
	}
}

//void ABGWeaponPickup::SetWeaponData(const FWeaponStatData& NewData)
//{
//	WeaponStatData = NewData;
//
//}
