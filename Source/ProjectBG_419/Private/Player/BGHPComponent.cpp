// Fill out your copyright notice in the Description page of Project Settings.

#include "BGHPComponent.h"
#include "BGPlayer.h"


// Sets default values for this component's properties
UBGHPComponent::UBGHPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHP = 100.f;
	CurrentHP = MaxHP;
	// ...
}


// Called when the game starts
void UBGHPComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBGHPComponent::InitializeComponent()
{
}


// Called every frame
void UBGHPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBGHPComponent::SetCurrentHP(float NewCurrentHP)
{
	if (NewCurrentHP < 0.f || NewCurrentHP > MaxHP)
	{
		return;
	}

	CurrentHP = NewCurrentHP;

	OnHPChanged.Broadcast();
}

void UBGHPComponent::AdjustCurrentHP(float NewHP)
{
	const float NewCurrentHP = CurrentHP + NewHP;
	if (NewHP < 0.f)
	{
		CurrentHP = FMath::Clamp<float>(NewCurrentHP, 0.f, NewCurrentHP);
		if (CurrentHP <= 0.f)
		{
			//Set Dead
			auto Player = Cast<ABGPlayer>(GetOwner());
			if (Player)
			{
				Player->SetCharacterState(ECharacterState::DEAD);
			}
		}
	}
	else
	{
		CurrentHP = FMath::Clamp<float>(NewCurrentHP, NewCurrentHP, MaxHP);
	}

	OnHPChanged.Broadcast();

}

void UBGHPComponent::SetMaxHP(float NewMaxHP)
{
	if (NewMaxHP < 0.f)
	{
		return;
	}

	if (NewMaxHP < CurrentHP)
	{
		CurrentHP = NewMaxHP;
	}
	
	MaxHP = NewMaxHP;
}

float UBGHPComponent::GetCurrentHP() const
{
	return CurrentHP;
}

float UBGHPComponent::GetMaxHP() const
{
	return MaxHP;
}

float UBGHPComponent::GetHPRatio() const
{
	return (CurrentHP <= 0.f) ? 0.f : CurrentHP / MaxHP;
}

bool UBGHPComponent::IsDead() const
{
	return (CurrentHP <= 0.f);
}

