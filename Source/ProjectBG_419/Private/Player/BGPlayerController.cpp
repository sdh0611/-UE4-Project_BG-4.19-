// Fill out your copyright notice in the Description page of Project Settings.

#include "BGPlayerController.h"
#include "BGUserWidget.h"
#include "BGShopWidget.h"
#include "BGInventoryWidget.h"
#include "BGHUD.h"
#include "BGPlayerCameraManager.h"
#include "ConstructorHelpers.h"

ABGPlayerController::ABGPlayerController()
{
	PlayerCameraManagerClass = ABGPlayerCameraManager::StaticClass();

	//static ConstructorHelpers::FClassFinder<UBGUserWidget>
	//	UI_HUD_C(TEXT("WidgetBlueprint'/Game/UI/UI_HUDWIdget.UI_HUDWIdget_C'"));
	//if (UI_HUD_C.Succeeded())
	//{
	//	HUDWidgetClass = UI_HUD_C.Class;
	//}

	//static ConstructorHelpers::FClassFinder<UBGShopWidget>
	//	UI_Shop(TEXT("WidgetBlueprint'/Game/UI/UI_ShopWidget.UI_ShopWidget_C'"));
	//if (UI_Shop.Succeeded())
	//{
	//	ShopWidgetClass = UI_Shop.Class;
	//}
	//else
	//{
	//	UE_LOG(LogClass, Error, TEXT("Shop ui not exist.."));
	//}
	//
}

void ABGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	//HUDWidget = CreateWidget<UBGUserWidget>(this, HUDWidgetClass);
	//HUDWidget->AddToViewport();

	//SetView
}

void ABGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ABGPlayerController::ToggleInventory);


}

ABGHUD * ABGPlayerController::GetBGHUD() const
{
	return Cast<ABGHUD>(GetHUD());
}

void ABGPlayerController::ToggleInventory()
{
	UE_LOG(LogClass, Warning, TEXT("ToggleInventory"));
	auto HUD = GetBGHUD();
	if (HUD->IsInventoryOnScreen())
	{
		HUD->RemoveInventoryWidgetOnScreen();
	}
	else
	{
		HUD->DrawInventoryWidgetOnScreen();
	}
}
