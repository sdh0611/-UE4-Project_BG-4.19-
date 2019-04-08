// Fill out your copyright notice in the Description page of Project Settings.

#include "BGWeaponStatComponent.h"


// Sets default values for this component's properties
UBGWeaponStatComponent::UBGWeaponStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Init default value
	Damage = 10.f;
	FireDelay = 0.2f;
	CurrentAmmo = 0.f;
	MaxAmmo = 30.f;
	// ...
}


// Called when the game starts
void UBGWeaponStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBGWeaponStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called every frame
void UBGWeaponStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBGWeaponStatComponent::SetWeaponCurrentAmmo(int32 NewCurrentAmmo)
{
	if (NewCurrentAmmo < 0)
	{
		return;
	}

	CurrentAmmo = NewCurrentAmmo;
}

void UBGWeaponStatComponent::SetWeaponMaxAmmo(int32 NewMaxAmmo)
{
	if (NewMaxAmmo < 0)
	{
		return;
	}

	MaxAmmo = NewMaxAmmo;
}

float UBGWeaponStatComponent::GetWeaponDamage() const
{
	return Damage;
}

float UBGWeaponStatComponent::GetWeaponFireDelay() const
{
	return FireDelay;
}

int32 UBGWeaponStatComponent::GetWeaponCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 UBGWeaponStatComponent::GetWeaponMaxAmmo() const
{
	return MaxAmmo;
}

