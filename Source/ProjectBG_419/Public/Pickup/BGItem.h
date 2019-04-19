// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "GameFramework/Actor.h"
#include "BGItem.generated.h"

DECLARE_DELEGATE(FOnItemInfoChanged);
DECLARE_MULTICAST_DELEGATE(FOnItemDropped);


UENUM(Blueprintable)
enum class EItemType : uint8
{
	WEAPON,
	RECOVERY,
	DOPING,
	DEFAULT,
	INVALID
};


static FString GetItemTypeEnumAsString(EItemType ItemType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemType"), true);
	if (nullptr == EnumPtr)
	{
		return FString(TEXT("Invalid"));
	}

	FString Concated = EnumPtr->GetNameStringByValue((int64)ItemType);
	Concated.RemoveFromStart(TEXT("EItemType::"));

	return Concated;
}

static EItemType GetItemTypeFromString(const FString& ItemTypeName)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemType"), true);
	if (nullptr == EnumPtr)
	{
		return EItemType::INVALID;
	}

	if (ItemTypeName == TEXT("WEAPON"))
	{
		return EItemType::WEAPON;
	}
	else if (ItemTypeName == TEXT("RECOVERY"))
	{
		return EItemType::RECOVERY;
	}
	else if (ItemTypeName == TEXT("DOPING"))
	{
		return EItemType::DOPING;
	}

	return EItemType::INVALID;
}

UCLASS()
class PROJECTBG_419_API ABGItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABGItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category=Item)
	virtual void OnUsed();

public:
	void AdjustItemNumber(int32 Value);
	virtual void InitItemData(struct FBGItemData* NewItemData);
	virtual void OnDropped();

public:
	void SetNumberOfItem(int32 NewItemNumber);
	void SetItemOwner(class ABGPlayer* NewItemOwner);
	

public:
	const class UTexture2D* GetItemTexture() const;
	const ABGPlayer* GetItemOwner() const;
	int32 GetNumberOfItem() const;
	int32 GetMaxNumberOfItem() const;
	int32 GetItemWeight() const;
	const FString& GetItemName() const;
	EItemType GetItemType() const;

public:
	FOnItemInfoChanged OnItemInfoChanged;

	FOnItemDropped OnItemDropped;

	UPROPERTY(VisibleAnywhere, Category = Item)
	TSubclassOf<class ABGPickup> PickupClass;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Item)
	class UTexture2D* ItemTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	class ABGPlayer* ItemOwner;

protected:
	//현재 아이템의 수량
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient)
	int32 NumberOfItem;

	//한꺼번에 들 수 있는 최대 수량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	int32 MaxNumberOfItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	int32 ItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	EItemType ItemType;

};
