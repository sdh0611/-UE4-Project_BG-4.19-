// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "Components/ActorComponent.h"
#include "BGHPComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBG_419_API UBGHPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBGHPComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetCurrentHP(float NewCurrentHP);
	void AdjustCurrentHP(float NewHP);
	void SetMaxHP(float NewMaxHP);

public:
	float GetCurrentHP() const;
	float GetMaxHP() const;
	float GetHPRatio() const;
	bool IsDead() const;

public:
	FOnHPChangedDelegate OnHPChanged;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP, Meta = (AllowPrivateAccess = true))
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HP, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	
};
