// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BGHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API ABGHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ABGHUD();

public:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	
public:
	void DrawInventoryWidgetOnScreen();
	void DrawShopWidgetOnScreen();
	void RemoveInventoryWidgetOnScreen();
	void RemoveShopWidgetOnScreen();

public:
	bool IsInventoryOnScreen() const;
	bool IsShopOnScreen() const;
	class UBGUserWidget* const GetUserWidget() const;
	class UBGShopWidget* const GetShopWidget() const;
	class UBGInventoryWidget* const GetInventoryWidget() const;

protected:
	bool bIsInventoryOnScreen;
	bool bIsShopOnScreen;

protected:
	//UPROPERTY(EditDefaultsOnly)
	//class UTexture2D* CrosshairTexture;

	UPROPERTY(VisibleAnywhere, Category = HUD)
	TSubclassOf<class UBGUserWidget> UserWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = HUD)
	TSubclassOf<class UBGShopWidget> ShopWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = HUD)
	TSubclassOf<class UBGInventoryWidget> InventoryWidgetClass;


private:
	//PlayerController
	UPROPERTY()
	class ABGPlayerController* PlayerController;

	//Widget classes
	UPROPERTY()
	class UBGUserWidget* UserWidget;

	UPROPERTY()
	class UBGShopWidget* ShopWidget;

	UPROPERTY()
	class UBGInventoryWidget* InventoryWidget;


};
