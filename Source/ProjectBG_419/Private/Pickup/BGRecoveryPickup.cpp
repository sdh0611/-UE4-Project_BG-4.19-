// Fill out your copyright notice in the Description page of Project Settings.

#include "BGRecoveryPickup.h"
#include "BGRecovery.h"
#include "BGPlayer.h"
#include "BGPlayerController.h"
#include "BGPlayerItemStatusComponent.h"
#include "Engine/World.h"

ABGRecoveryPickup::ABGRecoveryPickup()
{
	ItemClass = ABGRecovery::StaticClass();

}

void ABGRecoveryPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ABGRecoveryPickup::OnInteraction(APawn * Pawn)
{
	auto Player = Cast<ABGPlayer>(Pawn);
	if (Player)
	{
		auto PlayerItemStatus = Player->GetPlayerItemStatusComponent();
		if (PlayerItemStatus)
		{
			ABGRecovery* Recovery = GetWorld()->SpawnActor<ABGRecovery>(ItemClass);
			if (Recovery)
			{
				auto ItemStatus = Player->GetPlayerItemStatusComponent();
				ItemStatus->AddItem(Recovery);
				Recovery->SetItemOwner(Player);
				UE_LOG(LogClass, Warning, TEXT("Get Recovery Item!!"));
			}
		}

	}

	Super::OnInteraction(Pawn);
}

bool ABGRecoveryPickup::IsPlayerHasItem()
{
	return false;
}
