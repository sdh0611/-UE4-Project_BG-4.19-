// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "BGInteractionActor.h"
#include "BGShop.generated.h"


UCLASS()
class PROJECTBG_419_API ABGShop : public ABGInteractionActor
{
	enum class EShopState : uint8
	{
		SELL,
		BUY
	};

	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABGShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteraction(class APawn* Pawn) override;
	
public:
	//Player에게 판매
	void SellItemToPlayer(const FBGShopItemData& NewShopItem);
	//Player에게 구매
	void BuyItemFromPlayer(const FBGShopItemData& NewShopItem);
	void OnPlayerExitShop();
	const FBGShopItemData* const FindShopItemData(const FString& ItemName);

private:
	void ConstructShopWidget(class ABGPlayerController* PlayerController);

private:
	//TSubclassOf<class ABGWeapon> WeaponClass;
	UPROPERTY(EditAnywhere, Category = Shop, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class ABGItem> WeaponItemClass;

	UPROPERTY(EditAnywhere, Category = Shop, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class ABGItem> RecoveryItemClass;

	UPROPERTY()
	TArray<class ABGItem*> ItemList;

private:
	UPROPERTY(VisibleAnywhere, Category = Shop)
	class UStaticMeshComponent* ShopMesh;

	UPROPERTY()
	class UDataTable* ShopItemDataTable;

	UPROPERTY()
	class ABGPlayer* EnterPlayer;
};
