// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "Components/ActorComponent.h"
#include "BGPlayerItemStatusComponent.generated.h"

DECLARE_DELEGATE(FOnMoneyChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBG_419_API UBGPlayerItemStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBGPlayerItemStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void AddItem(class ABGItem* NewItem);
	void RemoveItem(class ABGItem* NewItem, bool bDestroy = true);
	void RemoveItem(const FString& ItemName, bool bDestroy = true);
	void RemoveItemEntire(const FString& ItemName, bool bDestroy = true);
	void RemoveItemEntire(class ABGItem* NewItem, bool bDestroy = true);
	void AddMoney(int32 NewMoney);
	//반환할 값은 Player가 들고있어야 할 무기의 포인터.
	class ABGWeapon* EquipWeapon(class ABGWeapon* NewWeapon);
	void RemoveWeapon(ABGWeapon* NewWeapon, bool bDestroy = true);



public:
	void SetMoney(int32 NewMoney);
	void SetCurrentWeaponIndex(int32 NewWeaponIndex);

public:
	int32 GetCurrentMoney() const;
	int32 GetCurrentWeaponIndex() const;
	int32 GetNumberOfItemList() const;
	class ABGItem* const GetItemByName(const FString& ItemName);
	class ABGItem* const GetItemByIndex(int32 ItemIndex);
	class ABGWeapon* const GetWeapon(int32 NewWeaponIndex);

public:
	FOnMoneyChanged OnMoneyChanged;

private:
	TArray<class ABGItem*> ItemList;

private:
	UPROPERTY()
	class ABGPlayer* OwnerPlayer;

	UPROPERTY(Transient, VisibleAnywhere, Category = Weapon)
	TArray<class ABGWeapon*> WeaponInventory;

private:
	int32 TotalWeight;
	int32 CurrentWeight;
	int32 CurrentMoney;
	int32 CurrentWeaponIndex;

};
