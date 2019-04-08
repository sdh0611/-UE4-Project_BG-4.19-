// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "Components/ActorComponent.h"
#include "BGWeaponStatComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBG_419_API UBGWeaponStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBGWeaponStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeaponCurrentAmmo(int32 NewCurrentAmmo);
	void SetWeaponMaxAmmo(int32 NewMaxAmmo);

public:
	float GetWeaponDamage() const;
	float GetWeaponFireDelay() const;
	int32 GetWeaponCurrentAmmo() const;
	int32 GetWeaponMaxAmmo() const;


private:
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat, Meta = (AllowPrivateAccess = true))
	FString WeaponName;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat, Meta = (AllowPrivateAccess = true))
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat, Meta = (AllowPrivateAccess = true))
	float FireDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat, Meta = (AllowPrivateAccess = true))
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat, Meta = (AllowPrivateAccess = true))
	int32 MaxAmmo;
};
