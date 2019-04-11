// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "BGItem.h"
#include "BGWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponFire);
DECLARE_DELEGATE(FOnCurrentAmmoVarying);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SMG,
	AR,
	SHOTGUN,
	DMR,
	SR,
	PISTOL,
	GRENADE,
	KNIFE,
	INVALID
};

////NOTE(03.19) : Consider how to handle weapon stat data
//USTRUCT(BlueprintType)
//struct FWeaponStatData
//{
//	GENERATED_BODY()
//
//public:
//	FWeaponStatData()
//		:WeaponName(TEXT("Default")), Damage(10.f), FireDelay(0.1f), MaxAmmo(30), WeaponType(EWeaponType::AR)
//	{
//	}
//
//public:
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
//	int32 MaxAmmo;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponStat)
//	EWeaponType WeaponType;
//};

static FString GetWeaponTypeEnumAsString(EWeaponType WeaponType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
	if (nullptr == EnumPtr)
	{
		return FString(TEXT("Invalid"));
	}
	
	FString Concated = EnumPtr->GetNameStringByValue((int64)WeaponType);
	Concated.RemoveFromStart(TEXT("EWeaponType::"));

	return Concated;
}

static EWeaponType GetWeaponTypeFromString(const FString& WeaponTypeName)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
	if (nullptr == EnumPtr)
	{
		return EWeaponType::INVALID;
	}

	if (WeaponTypeName == TEXT("SMG"))
	{
		return EWeaponType::SMG;
	}
	else if (WeaponTypeName == TEXT("AR"))
	{
		return EWeaponType::AR;
	}
	else if (WeaponTypeName == TEXT("SHOTGUN"))
	{
		return EWeaponType::SHOTGUN;
	}
	else if (WeaponTypeName == TEXT("DMR"))
	{
		return EWeaponType::DMR;
	}
	else if (WeaponTypeName == TEXT("SR"))
	{
		return EWeaponType::SR;
	}
	else if (WeaponTypeName == TEXT("PISTOL"))
	{
		return EWeaponType::PISTOL;
	}
	else if (WeaponTypeName == TEXT("GRENADE"))
	{
		return EWeaponType::GRENADE;
	}
	else if (WeaponTypeName == TEXT("KNIFE"))
	{
		return EWeaponType::KNIFE;
	}

	return EWeaponType::INVALID;
}

UCLASS()
class PROJECTBG_419_API ABGWeapon : public ABGItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABGWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void FireStart();
	virtual void FireEnd();

public:
	//UFUNCTION()
	void Reload();
	virtual void InitItemData(struct FBGItemData* NewItemData) override;
	void Dropped();

public:
	//void SetWeaponOwner(class ABGPlayer* Player);

public:
	bool IsCanReload() const;
	bool IsNeedToReload() const;
	int32 GetCurrentAmmo() const;
	EWeaponType GetWeaponType() const;
	const FString& GetWeaponName() const;

protected:
	FHitResult WeaponTrace(const FVector& Start, const FVector& End);

private:
	void Fire();
	FVector GetAdjustAim();
	void OnAssetLoadCompleted();

public:
	FOnWeaponFire OnWeaponFire;
	FOnCurrentAmmoVarying OnCurrentAmmoVarying;
	//TSubclassOf <class ABGWeaponPickup > PickupClass;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float FireDelay;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, Category = Weapon )
	float FireTimer;

	UPROPERTY(EditDefaultsOnly, Category = Weapon )
	int32 CurrentAmmo;

	UPROPERTY(VisibleAnywhere, Category = Weapon )
	bool bWantsToFire;

	UPROPERTY(VisibleAnywhere, Category = Weapon )
	FVector MuzzleLocation;

	UPROPERTY(EditDefaultsOnly, Category = Projectile )
	TSubclassOf<class ABGProjectile> ProjectileClass;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* WeaponMesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = true))
	//class ABGPlayer* WeaponOwner;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = true))
	//class UBGWeaponStatComponent* WeaponStat;

private:
	FSoftObjectPath WeaponAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

};
