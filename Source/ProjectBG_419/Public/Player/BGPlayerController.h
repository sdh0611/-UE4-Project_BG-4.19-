// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "GameFramework/PlayerController.h"
#include "BGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API ABGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABGPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	class ABGHUD* GetBGHUD() const;
	
protected:
	void ToggleInventory();

protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	//TSubclassOf<class UBGUserWidget> HUDWidgetClass;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	//TSubclassOf<class UBGShopWidget> ShopWidgetClass;
	

private:
	//UPROPERTY()
	//class UBGUserWidget* HUDWidget;

	//UPROPERTY()
	//class UBGShopWidget* ShopWidget;

};
