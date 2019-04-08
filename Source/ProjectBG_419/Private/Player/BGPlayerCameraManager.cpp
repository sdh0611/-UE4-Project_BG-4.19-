// Fill out your copyright notice in the Description page of Project Settings.

#include "BGPlayerCameraManager.h"
#include "BGPlayer.h"
#include "BGPlayerController.h"
#include "Kismet/GameplayStatics.h"


ABGPlayerCameraManager::ABGPlayerCameraManager()
{
	//ViewPitchMax = -0.f;
	ViewPitchMin = -80.f;

	NormalFOV = 90.f;
	AimFOV = 45.f;
	ZoomFOV = 10.f;

	CurrentFOV = NormalFOV;
}

void ABGPlayerCameraManager::InitializeFor(APlayerController * PC)
{
	Super::InitializeFor(PC);
	UE_LOG(LogClass, Warning, TEXT("Camera Manager InitializeFor"));
	auto BGPlayerController = Cast<ABGPlayerController>(PC);
	if (BGPlayerController)
	{
		UE_LOG(LogClass, Warning, TEXT("Camera Manager Init PC!"));
	}
	
}

void ABGPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	auto Player = Cast<ABGPlayer>(PCOwner->GetPawn());
	if (Player)
	{
		float TargetFOV = Player->IsAiming() ? AimFOV : NormalFOV;

		CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 20.f);
		SetFOV(CurrentFOV);
	}


}
