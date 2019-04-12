// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "GameFramework/Character.h"
#include "BGPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAimingDelegate);

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	READY,
	PLAY,
	DEAD
};

UCLASS()
class PROJECTBG_419_API ABGPlayer : public ACharacter
{
	GENERATED_BODY()

	enum class EViewMode
	{
		VIEW_TPS,
		VIEW_FREE,
		VIEW_FPS
	};	

public:
	// Sets default values for this character's properties
	ABGPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	bool IsAiming() const;
	bool IsSprinting() const;
	bool IsReloading() const;
	bool IsSwitchingWeapon() const;
	bool IsDead() const;
	class UBGPlayerItemStatusComponent* const GetPlayerItemStatusComponent() const;
	class UBGHPComponent* const GetHPComponent() const;
	class ABGInteractionActor* GetInteractionActorInView();
	class ABGWeapon* const GetCurrentWeapon() const;

public:
	void EquipWeapon(class ABGWeapon* NewWeapon);
	void SetCharacterState(ECharacterState NewState);
	void SetIsReloading(bool NewState);
	void SetIsSwitchingWeapon(bool NewState);

protected:
	void CheckRotation(float DeltaTime);

	//Axis Method
private:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	//Action Method
private:
	void ViewFree();
	void ViewFreeRelease();
	void ViewChange();
	void Aim();
	void AimRelease();
	void Sprint();
	void SprintRelease();
	void Interaction();
	void Attack();
	void AttackEnd();
	void ToggleCrouch();
	void DoJump();
	void Reload();
	void DropWeapon();
	
	////Switch weapon previous weapon
	////When switch weapon, play montage 'Equip weapon'.
	//void SwitchWeapon();
	//Switch weapon in slots
	void Slot1();
	void Slot2();
	//void Slot3();
	//void Slot4();
	//void Slot5();

	void ToggleInventory();

	//For Debug
	void DamageSelf();
	void ShowMeTheMoney();

	void ChangeViewMode(EViewMode NewMode);
	void ReloadingWeapon();
	void RemoveWeapon(class ABGWeapon* Weapon, bool bDestroy = true);
	//void SwitchingWeapon();
	//void DoSwitching();
	

public:
	FOnAimingDelegate OnAimingDelegate;

private:
	bool bIsAiming;
	bool bIsSprinting;
	bool bIsCrouching;
	bool bIsReloading;
	bool bIsSwitchingWeapon;
	float WalkSpeed;
	float SprintSpeed;
	float AimSpeed;
	float CrouchSpeed;
	//int32 CurrentWeaponIndex;
	FVector DirectionVector;
	FRotator Rotation;
	EViewMode CurrentViewMode = EViewMode::VIEW_TPS;
	ECharacterState CurrentCharacterState = ECharacterState::READY;

public:
	static const FName MainWeaponSocket;
	static const FName SecondWeaponSocket;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* AimSpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* AimCamera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* ScopeCamera;

	UPROPERTY(VisibleAnywhere, Category = HP)
	class UBGHPComponent* HPComponent;

	UPROPERTY(VisibleAnywhere, Category = HP)
	class UBGPlayerItemStatusComponent* ItemStatusComponent;

private:
	UPROPERTY(VisibleAnywhere, Category = Pickup)
	class ABGInteractionActor* CurrentInteractionActorInView;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ABGWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ABGWeapon* PreviousWeapon;

	//UPROPERTY(Transient, VisibleAnywhere, Category = Weapon)
	//TArray<class ABGWeapon*> WeaponInventory;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	class UBGPlayerAnimInstance* AnimInstance;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	class ABGPlayerController* BGPlayerController;

};
