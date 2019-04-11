// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "BGType.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API UBGType : public UObject
{
	GENERATED_BODY()	
};

USTRUCT(BlueprintType)
struct FBGItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FBGItemData()
		:ItemName(TEXT("Default")), ItemType(TEXT("DEFAULT")), ItemWeight(0), ItemImagePath(TEXT("")), StaticMeshPath(TEXT(""))
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	int32 ItemWeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemData)
	FString ItemImagePath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString StaticMeshPath;
};

USTRUCT(BlueprintType)
struct FBGWeaponData : public FBGItemData
{
	GENERATED_BODY()

public:
	FBGWeaponData()
		: FBGItemData(), Damage(0.f), FireDelay(0.f), MaxAmmo(30), WeaponType(TEXT("Default")), SkeletalMeshPath(TEXT(""))
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	float FireDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString WeaponType;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	//FString StaticMeshPath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString SkeletalMeshPath;

};

USTRUCT(BlueprintType)
struct FBGRecoveryData : public FBGItemData
{
	
	GENERATED_BODY()

public:
	FBGRecoveryData()
		: FBGItemData(), RecoveryAmount(0.f), RecoveryDelay(0.f)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RecoveryData)
	float RecoveryAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = RecoveryData)
	float RecoveryDelay;

};

//USTRUCT(BlueprintType)
//struct FBGWeaponData : public FTableRowBase
//{
//	GENERATED_BODY()
//
//public:
//	FBGWeaponData()
//		:WeaponName(TEXT("Default")), Damage(1.f), FireDelay(0.2f), MaxAmmo(30), WeaponType(TEXT("Default")),
//		StaticMeshPath(TEXT("")), SkeletalMeshPath(TEXT(""))
//	{
//	}
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponStat)
//		FString WeaponName;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponStat)
//		float Damage;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponStat)
//		float FireDelay;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponStat)
//		int32 MaxAmmo;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponStat)
//		FString WeaponType;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponStat)
//		FString StaticMeshPath;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponStat)
//		FString SkeletalMeshPath;
//
//};

USTRUCT(BlueprintType)
struct FBGShopItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FBGShopItemData()
		: ItemName(TEXT("Default")), ItemPrice(0), ItemExplanation(TEXT("Default")), ItemType(TEXT("Default")), ItemImagePath(TEXT(""))
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShopItemData)
	FString ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShopItemData)
	int32 ItemPrice;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShopItemData)
	FString ItemExplanation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShopItemData)
	FString ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShopItemData)
	FString ItemImagePath;


};

