// Fill out your copyright notice in the Description page of Project Settings.

#include "BGHUD.h"
#include "BGUserWidget.h"
#include "BGInventoryWidget.h"
#include "BGShopWidget.h"
#include "BGPlayerController.h"
#include "Engine/Canvas.h"
#include "ConstructorHelpers.h"

ABGHUD::ABGHUD()
{
	//static ConstructorHelpers::FObjectFinder<UTexture2D>
	//	TEXTURE_Crosshair(TEXT("Texture2D'/Game/Crosshair_fps_tutorial/crosshair.crosshair'"));
	//if (TEXTURE_Crosshair.Succeeded())
	//{
	//	CrosshairTexture = TEXTURE_Crosshair.Object;
	//}

	static ConstructorHelpers::FClassFinder<UBGUserWidget>
		UI_HUD_C(TEXT("WidgetBlueprint'/Game/UI/UI_HUDWIdget.UI_HUDWIdget_C'"));
	if (UI_HUD_C.Succeeded())
	{
		UserWidgetClass = UI_HUD_C.Class;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("HUD ui not exist.."));
	}

	static ConstructorHelpers::FClassFinder<UBGShopWidget>
		UI_Shop(TEXT("WidgetBlueprint'/Game/UI/UI_ShopWidget.UI_ShopWidget_C'"));
	if (UI_Shop.Succeeded())
	{
		ShopWidgetClass = UI_Shop.Class;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Shop ui not exist.."));
	}

	static ConstructorHelpers::FClassFinder<UBGInventoryWidget>
		UI_Inventory(TEXT("WidgetBlueprint'/Game/UI/UI_UserInventory.UI_UserInventory_C'"));
	if (UI_Inventory.Succeeded())
	{
		InventoryWidgetClass = UI_Inventory.Class;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Inventory ui not exist.."));
	}

	PlayerController = nullptr;
	bIsInventoryOnScreen = false;
	bIsShopOnScreen = false;

}


void ABGHUD::BeginPlay()
{
	PlayerController = Cast<ABGPlayerController>(PlayerOwner);
	
	//Pooling widgets
	if (PlayerController)
	{
		UserWidget = CreateWidget<UBGUserWidget>(PlayerController, UserWidgetClass);
		UserWidget->AddToViewport();

		ShopWidget = CreateWidget<UBGShopWidget>(PlayerController, ShopWidgetClass);
		ShopWidget->AddToViewport();
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);

		InventoryWidget = CreateWidget<UBGInventoryWidget>(PlayerController, InventoryWidgetClass);
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("HUD init fail.."));
	}

}

void ABGHUD::DrawHUD()
{
	Super::DrawHUD();

	//if (CrosshairTexture)
	//{
	//	FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	//	FVector2D CrosshairDrawPosition(Center.X - CrosshairTexture->GetSurfaceWidth() * 0.5f, Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

	//	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
	//	TileItem.BlendMode = SE_BLEND_Translucent;
	//	Canvas->DrawItem(TileItem);
	//}

}

void ABGHUD::DrawInventoryWidgetOnScreen()
{

	InventoryWidget->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(GetInventoryWidget()->GetCachedWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;

	bIsInventoryOnScreen = true;

}

void ABGHUD::DrawShopWidgetOnScreen()
{
	ShopWidget->SetVisibility(ESlateVisibility::Visible);
	
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(GetShopWidget()->GetCachedWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;

	bIsShopOnScreen = true;

}

void ABGHUD::RemoveInventoryWidgetOnScreen()
{
	//InventoryWidget->RemoveFromParent();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	bIsInventoryOnScreen = false;

}

void ABGHUD::RemoveShopWidgetOnScreen()
{
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
	
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	bIsShopOnScreen = false;

}

bool ABGHUD::IsInventoryOnScreen() const
{
	return bIsInventoryOnScreen;
}

bool ABGHUD::IsShopOnScreen() const
{
	return bIsShopOnScreen;
}

UBGUserWidget * const ABGHUD::GetUserWidget() const
{
	return UserWidget;
}

UBGShopWidget * const ABGHUD::GetShopWidget() const
{
	return ShopWidget;
}

UBGInventoryWidget * const ABGHUD::GetInventoryWidget() const
{
	return InventoryWidget;
}
