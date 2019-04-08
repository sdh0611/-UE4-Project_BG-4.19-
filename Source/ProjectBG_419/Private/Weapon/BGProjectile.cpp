// Fill out your copyright notice in the Description page of Project Settings.

#include "BGProjectile.h"
#include "BGPlayerController.h"
#include "BGPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ConstructorHelpers.h"
#include "TimerManager.h"

// Sets default values
ABGProjectile::ABGProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->InitSphereRadius(5.f);
	Sphere->SetCollisionProfileName(TEXT("Bullet"));

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Movement->SetUpdatedComponent(Sphere);
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 3000.f;
	Movement->bRotationFollowsVelocity = true;
	//For Test
	Movement->ProjectileGravityScale = 0.f;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(Sphere);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_Projectile(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45.SM_Shell_556x45'"));
	if (SM_Projectile.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(SM_Projectile.Object);
	}
	Sphere->OnComponentHit.AddDynamic(this, &ABGProjectile::OnHit);

	Damage = 10.f;
	LifeSpan = 3.f;
}

// Called when the game starts or when spawned
void ABGProjectile::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogClass, Warning, TEXT("%s"), *Instigator->GetName());
	SetLifeSpan(LifeSpan);
}

// Called every frame
void ABGProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGProjectile::FireInDirection(const FVector & Direction)
{
	if (Movement)
	{
		Movement->Velocity = Direction * Movement->InitialSpeed;
	}
}

void ABGProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor)
	{
		UE_LOG(LogClass, Warning, TEXT("HitActorName : %s"), *OtherActor->GetName());
		//Instigator->GetName();
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(Damage, DamageEvent, Instigator->GetController(), this);
		Destroy();
	}
	Destroy();

}

