// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "GameFramework/HUD.h"
#include "BGCrosshair.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API ABGCrosshair : public AHUD
{
	GENERATED_BODY()
		
public:
	ABGCrosshair();
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairTexture;
	
};
