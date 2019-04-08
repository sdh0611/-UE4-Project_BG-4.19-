// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "GameFramework/Actor.h"
#include "BGProjectile.generated.h"

UCLASS()
class PROJECTBG_419_API ABGProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABGProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void FireInDirection(const FVector& Direction);

public:
	UFUNCTION()
		void OnHit(class UPrimitiveComponent* HitComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComponent,
			FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class UProjectileMovementComponent* Movement;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	class UStaticMeshComponent* ProjectileMesh;

private:
	UPROPERTY(EditDefaultsOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float LifeSpan;


};
