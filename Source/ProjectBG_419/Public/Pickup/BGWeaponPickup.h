// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
//#include "BGGameInstance.h"
#include "BGPickup.h"
#include "BGWeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API ABGWeaponPickup : public ABGPickup
{
	GENERATED_BODY()
	
public:
	ABGWeaponPickup();

public:
	virtual void PostInitializeComponents() override;

public:
	virtual void OnInteraction(class APawn* Pawn) override;

public:
	//void SetWeaponData(const FWeaponStatData& NewData);
	//void InitWeaponPickup(class ABGWeapon* NewWeapon);
	void SetWeapon(class ABGWeapon* NewWeapon);

private:
	//TSubclassOf<class ABGWeapon> WeaponClass;
	//FWeaponStatData WeaponStatData;
	//struct FBGItemData* WeaponData;
	// Pickup단에서 생성할 Item Actor의 데이터 구조체를 SharedPtr의 형태로 공유할 수 있게끔 수정하자.
	//TSharedPtr<FBGItemData> WeaponData;

	UPROPERTY(VisibleAnywhere, Category=WeaponPickup)
	class ABGWeapon* Weapon;
};
