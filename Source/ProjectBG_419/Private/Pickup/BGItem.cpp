// Fill out your copyright notice in the Description page of Project Settings.

#include "BGItem.h"


// Sets default values
ABGItem::ABGItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Init properties;
	ItemTexture = nullptr;
	ItemOwner = nullptr;
	NumberOfItem = 1;
	MaxNumberOfItem = 999;
	ItemWeight = 0;
	ItemName = TEXT("Default");
	ItemType = EItemType::DEFAULT;
}

// Called when the game starts or when spawned
void ABGItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABGItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGItem::OnUsed()
{
	AdjustItemNumber(-1);
	//if (--NumberOfItem <= 0)
	//{
	//	UE_LOG(LogClass, Warning, TEXT("Item Destroy"));
	//	Destroy();
	//}
	//else
	//{
	//	UE_LOG(LogClass, Warning, TEXT("Item Decrease"));
	//	OnItemInfoChanged.Execute();
	//}
}

void ABGItem::AdjustItemNumber(int32 Value)
{
	int32 NewNumberOfItem = NumberOfItem + Value;

	NumberOfItem = FMath::Clamp<int32>(NewNumberOfItem, 0, NewNumberOfItem);

	OnItemInfoChanged.Execute();

	if (NumberOfItem <= 0)
	{
		Destroy();
	}
}

void ABGItem::InitItemData(FBGItemData * NewItemData)
{
	if (NewItemData)
	{
		ItemName = NewItemData->ItemName;
		ItemType = GetItemTypeFromString(NewItemData->ItemType);
		ItemWeight = NewItemData->ItemWeight;
	}
}

void ABGItem::OnDropped()
{
	OnItemDropped.Broadcast();
}

void ABGItem::SetNumberOfItem(int32 NewItemNumber)
{
	if (NewItemNumber < 0)
	{
		return;
	}

	NumberOfItem = NewItemNumber;
}

void ABGItem::SetItemOwner(ABGPlayer * NewItemOwner)
{
	ItemOwner = NewItemOwner;
}

const UTexture2D * ABGItem::GetItemTexture() const
{
	return ItemTexture;
}

const ABGPlayer * ABGItem::GetItemOwner() const
{
	return ItemOwner;
}

int32 ABGItem::GetNumberOfItem() const
{
	return NumberOfItem;
}

int32 ABGItem::GetMaxNumberOfItem() const
{
	return MaxNumberOfItem;
}

int32 ABGItem::GetItemWeight() const
{
	return ItemWeight;
}

const FString& ABGItem::GetItemName() const
{
	return ItemName;
}

EItemType ABGItem::GetItemType() const
{
	return ItemType;
}


