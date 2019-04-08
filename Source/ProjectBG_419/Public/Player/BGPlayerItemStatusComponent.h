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
	void AddMoney(int32 NewMoney);

public:
	void SetMoney(int32 NewMoney);

public:
	int32 GetCurrentMoney() const;
	class ABGItem* const IsContainItem(TSubclassOf<class ABGItem> ItemClassInfo);

public:
	FOnMoneyChanged OnMoneyChanged;

private:
	TArray<class ABGItem*> ItemList;

private:
	UPROPERTY()
	class ABGPlayer* OwnerPlayer;

private:
	int32 TotalWeight;
	int32 CurrentWeight;
	int32 CurrentMoney;
	
};
