// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "Camera/PlayerCameraManager.h"
#include "BGPlayerCameraManager.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTBG_419_API ABGPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	ABGPlayerCameraManager();
	
public:
	virtual void InitializeFor(APlayerController* PC) override;
	virtual void UpdateCamera(float DeltaTime) override;
	
public:

private:
	float NormalFOV;
	float AimFOV;
	float ZoomFOV;
	float CurrentFOV;

};
