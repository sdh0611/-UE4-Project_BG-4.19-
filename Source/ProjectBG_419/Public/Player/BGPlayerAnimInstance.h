// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "Animation/AnimInstance.h"
#include "BGPlayerAnimInstance.generated.h"

DECLARE_DELEGATE(FOnReloadEndDelegate);

DECLARE_DELEGATE(FOnSwitchDelegate);

/**
 * 
 */
UCLASS()
class PROJECTBG_419_API UBGPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBGPlayerAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;

public:
	void SetIsAiming(bool NewState);
	void SetIsEquipWeapon(bool NewState);
	void PlayFireMontage();
	void PlayMontage(const FString& MontageName);
	void SetCurrentMontage(const FString& MontageName);

public:
	bool IsMontagePlaying(const FString& MontageName);

private:
	UFUNCTION()
	void AnimNotify_ReloadEndCheck();

	UFUNCTION()
	void AnimNotify_SwitchWeapon();

	UFUNCTION()
	void AnimNotify_SwitchWeaponEnd();

public:
	FOnReloadEndDelegate OnReloadEnd;
	FOnSwitchDelegate OnSwitch;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta=(AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsEquipWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float fAimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float fAimYaw;
	
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* CurrentPlayMontage;

	TMap<FString, class UAnimMontage*> MontageTable;

};
