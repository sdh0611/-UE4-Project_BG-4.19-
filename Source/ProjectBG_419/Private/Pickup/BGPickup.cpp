// Fill out your copyright notice in the Description page of Project Settings.

#include "BGPickup.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABGPickup::ABGPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	bIsActive = true;
}

// Called when the game starts or when spawned
void ABGPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABGPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGPickup::OnInteraction(APawn* Pawn)
{
	Destroy();
}

void ABGPickup::OnFocus()
{
	Mesh->SetRenderCustomDepth(true);
	//UE_LOG(LogClass, Warning, TEXT("OnFocus!!"));
}

void ABGPickup::OnFocusEnd()
{
	Mesh->SetRenderCustomDepth(false);
	//UE_LOG(LogClass, Warning, TEXT("Focus Out."));
}

bool ABGPickup::IsActive() const
{
	return bIsActive;
}

void ABGPickup::SetActive(bool NewState)
{
	bIsActive = NewState;
	if (bIsActive)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Mesh->SetVisibility(true);
	}
	else
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetVisibility(false);
	}

}

