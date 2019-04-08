// Fill out your copyright notice in the Description page of Project Settings.

#include "BGCrosshair.h"
#include "Engine/Canvas.h"
#include "ConstructorHelpers.h"

ABGCrosshair::ABGCrosshair()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D>
		TEXTURE_Crosshair(TEXT("Texture2D'/Game/Crosshair_fps_tutorial/crosshair.crosshair'"));
	if (TEXTURE_Crosshair.Succeeded())
	{
		CrosshairTexture = TEXTURE_Crosshair.Object;
	}

}

void ABGCrosshair::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture)
	{
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		FVector2D CrosshairDrawPosition(Center.X - CrosshairTexture->GetSurfaceWidth() * 0.5f, Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}


}

