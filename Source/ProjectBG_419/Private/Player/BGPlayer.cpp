// Fill out your copyright notice in the Description page of Project Settings.

#include "BGPlayer.h"
#include "BGPlayerController.h"
#include "BGPlayerAnimInstance.h"
#include "BGPickup.h"
#include "BGWeapon.h"
#include "BGWeaponPickup.h"
#include "BGHPComponent.h"
#include "BGPlayerItemStatusComponent.h"
#include "BGUserWidget.h"
#include "BGInventoryWidget.h"
#include "BGWeaponInventoryWidget.h"
#include "BGHUD.h"
#include "BGGameInstance.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimationAsset.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"

const FName ABGPlayer::MainWeaponSocket = FName(TEXT("weapon_r"));
const FName ABGPlayer::SecondWeaponSocket = FName(TEXT("second_weapon"));

// Sets default values
ABGPlayer::ABGPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_PLAYER(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_PLAYER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_PLAYER.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Set AnimInstance
	static ConstructorHelpers::FClassFinder<UBGPlayerAnimInstance>
		PLAYER_ANIM(TEXT("AnimBlueprint'/Game/Animation/BGPlayerAnimBlueprint.BGPlayerAnimBlueprint_C'"));
	if (PLAYER_ANIM.Succeeded())
	{
		UE_LOG(LogClass, Warning, TEXT("Setup AnimInstance!"));
		GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);

	}

	bUseControllerRotationYaw = false;

	//TPS Camera boom
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 200.f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 64.f), FRotator(-20.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 800.f;
	CurrentViewMode = EViewMode::VIEW_TPS;

	//TPS Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(0.f, 50.f, 0.f));

	//AimSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("AimSpringArm"));
	//AimSpringArm->SetupAttachment(GetCapsuleComponent());
	//AimSpringArm->TargetArmLength = 100.f;
	//AimSpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 64.f), FRotator(-20.f, ))

	//ScopeCamera
	ScopeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ScopeCamera"));
	ScopeCamera->SetupAttachment(GetCapsuleComponent());
	FVector EyeLoc;
	FRotator EyeRot;
	GetActorEyesViewPoint(EyeLoc, EyeRot);
	//ScopeCamera->SetRelativeLocationAndRotation(FVector(20.f, 0.f, 54.f), EyeRot);
	ScopeCamera->SetRelativeLocationAndRotation(EyeLoc, EyeRot);
	ScopeCamera->bUsePawnControlRotation = true;
	ScopeCamera->SetActive(false);

	//HPComponent
	HPComponent = CreateDefaultSubobject<UBGHPComponent>(TEXT("HPComponent"));
	HPComponent->SetMaxHP(100.f);
	HPComponent->SetCurrentHP(100.f);

	//ItemStatusComponent
	ItemStatusComponent = CreateDefaultSubobject<UBGPlayerItemStatusComponent>(TEXT("ItemStatusComponent"));

	//Player properties
	bIsAiming = false;
	bIsSprinting = false;
	bIsCrouching= false;
	WalkSpeed = 500.f;
	SprintSpeed = 800.f;
	AimSpeed = 150.f;
	CrouchSpeed = 250.f;
	CurrentCharacterState = ECharacterState::READY;
	CurrentInteractionActorInView = nullptr;
	CurrentWeapon = nullptr;
	PreviousWeapon = nullptr;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	DirectionVector = FVector::ZeroVector;
	Rotation = FRotator::ZeroRotator;

	//WeaponInventory.Init(nullptr, 5);
}

// Called when the game starts or when spawned
void ABGPlayer::BeginPlay()
{
	Super::BeginPlay();
	CurrentCharacterState = ECharacterState::PLAY;
	auto PlayerController = Cast<ABGPlayerController>(GetController());
	if (PlayerController)
	{
		BGPlayerController = PlayerController;
		BGPlayerController->GetBGHUD()->GetUserWidget()->BindCharacterHP(HPComponent);
		BGPlayerController->GetBGHUD()->GetUserWidget()->BindPlayerItemStatus(ItemStatusComponent);
	}

	//AnimInstance->OnSwitch.BindUObject(this, &ABGPlayer::DoSwitching);

	//GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("AnimSequence'/Game/AnimStarterPack/Idle_Rifle_Hip.Idle_Rifle_Hip'"));
	//if (AnimAsset != nullptr)
	//{
	//	GetMesh()->PlayAnimation(AnimAsset, true);
	//}
	//ChangeViewMode(EViewMode::VIEW_TPS);
}

// Called every frame
void ABGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABGInteractionActor* InteractionActor = GetInteractionActorInView();

	if (CurrentInteractionActorInView)
	{
		CurrentInteractionActorInView->OnFocusEnd();
	}
	
	if (InteractionActor)
	{
		InteractionActor->OnFocus();
	}
	CurrentInteractionActorInView = InteractionActor;
	
	if (CurrentViewMode == EViewMode::VIEW_TPS && !IsSprinting() && !IsAiming())
	{
		if (GetVelocity().Size() <= 1.f)
		{
			bUseControllerRotationYaw = false;
		}
		else
		{
			if (!IsSprinting())
			{
				bUseControllerRotationYaw = true;
			}
		}
	}

	//if (CurrentViewMode != EViewMode::VIEW_FREE)
	//{
	//	CheckRotation(DeltaTime);
	//}
}

void ABGPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	auto BGAnimInstance = Cast<UBGPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (BGAnimInstance)
	{
		AnimInstance = BGAnimInstance;
		AnimInstance->OnReloadEnd.BindUObject(this, &ABGPlayer::ReloadingWeapon);
	}


}

// Called to bind functionality to input
void ABGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind Axis Input
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABGPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABGPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABGPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABGPlayer::Turn);
	
	//Bind Action Input
	PlayerInputComponent->BindAction(TEXT("ViewFree"), IE_Pressed, this, &ABGPlayer::ViewFree);
	PlayerInputComponent->BindAction(TEXT("ViewFree"), IE_Released, this, &ABGPlayer::ViewFreeRelease);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), IE_Pressed, this, &ABGPlayer::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABGPlayer::DoJump);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ABGPlayer::Aim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ABGPlayer::AimRelease);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ABGPlayer::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ABGPlayer::SprintRelease);
	PlayerInputComponent->BindAction(TEXT("InterAction"), IE_Pressed, this, &ABGPlayer::Interaction);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABGPlayer::Attack);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ABGPlayer::AttackEnd);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ABGPlayer::ToggleCrouch);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABGPlayer::Reload);
	//PlayerInputComponent->BindAction(TEXT("SwitchWeapon"), IE_Pressed, this, &ABGPlayer::SwitchWeapon);
	PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &ABGPlayer::DropWeapon);
	PlayerInputComponent->BindAction(TEXT("Slot1"), IE_Pressed, this, &ABGPlayer::Slot1);
	PlayerInputComponent->BindAction(TEXT("Slot2"), IE_Pressed, this, &ABGPlayer::Slot2);
	//PlayerInputComponent->BindAction(TEXT("Slot3"), IE_Pressed, this, &ABGPlayer::Slot3);
	//PlayerInputComponent->BindAction(TEXT("Slot4"), IE_Pressed, this, &ABGPlayer::Slot4);
	//PlayerInputComponent->BindAction(TEXT("Slot5"), IE_Pressed, this, &ABGPlayer::Slot5);
	//PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ABGPlayer::ToggleInventory);


	//For Debug
	PlayerInputComponent->BindAction(TEXT("DamageSelf"), IE_Pressed, this, &ABGPlayer::DamageSelf);
	PlayerInputComponent->BindAction(TEXT("ShowMeTheMoney"), IE_Pressed, this, &ABGPlayer::ShowMeTheMoney);
	
}

float ABGPlayer::TakeDamage(float DamageAmount, const FDamageEvent & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//if (HPComponent->GetCurrent > 0.f)
	//{
	//	CurrentHP = FMath::Clamp<float>(CurrentHP - FinalDamage, 0.f, CurrentHP - FinalDamage);
	//	if (CurrentHP <= 0.f)
	//	{
	//		SetCharacterState(ECharacterState::ECS_DEAD);
	//	}
	//}	

	//if (EventInstigator == GetController())
	//{
	//	FinalDamage = 0.f;
	//}

	HPComponent->AdjustCurrentHP(-FinalDamage);

	UE_LOG(LogClass, Warning, TEXT("PlayerHP : %f"), HPComponent->GetCurrentHP());

	return FinalDamage;
}

bool ABGPlayer::IsAiming() const
{
	return bIsAiming;
}

bool ABGPlayer::IsSprinting() const
{
	return bIsSprinting;
}

bool ABGPlayer::IsReloading() const
{
	return bIsReloading;
}

bool ABGPlayer::IsSwitchingWeapon() const
{
	return bIsSwitchingWeapon;
}

bool ABGPlayer::IsDead() const
{
	return (CurrentCharacterState == ECharacterState::DEAD);
}

 UBGPlayerItemStatusComponent * const ABGPlayer::GetPlayerItemStatusComponent() const
{
	return ItemStatusComponent;
}

 UBGHPComponent * const ABGPlayer::GetHPComponent() const
 {
	 return HPComponent;
 }

//WeaponTag에 따라 다른 슬롯에 배정되도록
void ABGPlayer::EquipWeapon(ABGWeapon * NewWeapon)
{
	if (nullptr == NewWeapon)
	{
		return;
	}

	//Set weapon owner
	NewWeapon->SetItemOwner(this);

	// Weapon 포인터가 가리키는 값은 Player가 들게 될 무기의 포인터값이다.
	ABGWeapon* Weapon = ItemStatusComponent->EquipWeapon(NewWeapon);
	if (Weapon)
	{
		// 이미 주무기 슬롯(1, 2번)이 다 차있는 경우.
		if (nullptr != CurrentWeapon)
		{
			UE_LOG(LogClass, Warning, TEXT("DropWeapon"));
			//ItemStatusComponent->RemoveItem(Weapon, false);
			DropWeapon();
		}

		CurrentWeapon = Weapon;
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainWeaponSocket);
	}
	else
	{
		//SecondWeapon Socket에 붙임.
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SecondWeaponSocket);
	}
	
	AnimInstance->SetIsEquipWeapon(true);

	if (BGPlayerController)
	{
		BGPlayerController->GetBGHUD()->GetUserWidget()->BindWeaponInfo(CurrentWeapon);
		UE_LOG(LogClass, Warning, TEXT("Index : %d"), NewWeapon->GetWeaponInventoryIndex());
			BGPlayerController->GetBGHUD()->GetInventoryWidget()->
			GetWeaponInvnetoryWidget(NewWeapon->GetWeaponInventoryIndex())->BindWeapon(NewWeapon);
	}

}

void ABGPlayer::SetCharacterState(ECharacterState NewState)
{
	switch (NewState)
	{
		case ECharacterState::READY:
		{

			break;
		}
		case ECharacterState::PLAY:
		{

			break;
		}
		case ECharacterState::DEAD:
		{
			UE_LOG(LogClass, Warning, TEXT("Player Dead"));
			GetMesh()->SetSimulatePhysics(true);
			

			//InputComponent->SetActive(false);
			//TSubclassOf<UInputComponent> Input;
			//GetComponentByClass(Input)->SetActive(false);
			//auto PlayerController = Cast<ABGPlayerController>(GetController());
			//if (PlayerController)
			//{
			//	PlayerController->SetCinematicMode(true, false, false, true, true);
			//}
			break;
		}
	}

	CurrentCharacterState = NewState;

}

void ABGPlayer::SetIsReloading(bool NewState)
{
	bIsReloading = NewState;
}

void ABGPlayer::SetIsSwitchingWeapon(bool NewState)
{
	bIsSwitchingWeapon = NewState;
}


void ABGPlayer::CheckRotation(float DeltaTime)
{
	const FRotator ControllerRotation = GetController()->GetControlRotation().Clamp();
	FRotator ActorRotation = GetActorRotation().Clamp();
	float Delta = FMath::Abs<float>(ActorRotation.Yaw - ControllerRotation.Yaw);
	
	bool bResult = FMath::IsWithinInclusive<float>(Delta, 0.f, 90.f);
	if (!bResult)
	{
		//UE_LOG(LogClass, Warning, TEXT("Change"));
		float Yaw = FMath::FInterpTo(ActorRotation.Yaw, ControllerRotation.Yaw, DeltaTime, 5.f);
		ActorRotation.Yaw = Yaw;
		SetActorRotation(ActorRotation);
		//ActorRotation.Yaw = ControllerRotation.Yaw;
		//SetActorRotation(ActorRotation);
	}

}

void ABGPlayer::MoveForward(float NewAxisValue)
{
	if ((Controller != NULL) && (NewAxisValue != 0.0f))
	{
		if (CurrentViewMode != EViewMode::VIEW_FREE)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			DirectionVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		// get forward vector
		if (NewAxisValue < 0.f)
		{
			if (IsSprinting())
			{
				SprintRelease();
			}
		}

		AddMovementInput(DirectionVector, NewAxisValue);
	}
}

void ABGPlayer::MoveRight(float NewAxisValue)
{
	if ((Controller != NULL) && (NewAxisValue != 0.f))
	{
		FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), NewAxisValue);
	}
	
}

void ABGPlayer::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void ABGPlayer::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void ABGPlayer::ViewFree()
{
	if (CurrentViewMode == EViewMode::VIEW_TPS)
	{
		Rotation = DirectionVector.Rotation();
		ChangeViewMode(EViewMode::VIEW_FREE);
	}

}

void ABGPlayer::ViewFreeRelease()
{
	if (CurrentViewMode == EViewMode::VIEW_FREE)
	{
		SpringArm->SetRelativeRotation(Rotation);
		ChangeViewMode(EViewMode::VIEW_TPS);
	}
}

void ABGPlayer::ViewChange()
{
	if (CurrentViewMode == EViewMode::VIEW_TPS)
	{
		ChangeViewMode(EViewMode::VIEW_FPS);
	}
	else if (CurrentViewMode == EViewMode::VIEW_FPS)
	{
		ChangeViewMode(EViewMode::VIEW_TPS);
	}

}

void ABGPlayer::Aim()
{
	if (!GetCharacterMovement()->IsFalling() && !IsReloading() && !IsSwitchingWeapon() && (nullptr != CurrentWeapon))
	{
		if (IsSprinting())
		{
			SprintRelease();
		}
		
		bUseControllerRotationYaw = true;
		bIsAiming = true;

		if (bIsCrouching)
		{
			GetCharacterMovement()->MaxWalkSpeedCrouched = AimSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = AimSpeed;
		}

		auto BGAnim = Cast<UBGPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		if (BGAnim)
		{
			BGAnim->SetIsAiming(bIsAiming);
		}
	}
}

void ABGPlayer::AimRelease()
{
	bIsAiming = false;
	bUseControllerRotationYaw = false;
	if (bIsCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	AnimInstance->SetIsAiming(bIsAiming);
}

void ABGPlayer::Sprint()
{
	if (!IsAiming() && !IsAiming() && 
		!IsReloading() && !IsSwitchingWeapon() &&
		!GetCharacterMovement()->IsFalling())
	{
		if (!IsSprinting())
		{
			////만약 방향이 -45~45일 경우
			//const FRotator ActorRotation = GetVelocity().Rotation().Clamp();
			//if (FMath::IsWithinInclusive<float>(ActorRotation.Yaw, -45.f, 45.f))
			//{
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bIsAiming = false;
			bIsSprinting = true;
			//}
		}
	}
}

void ABGPlayer::SprintRelease()
{
	if (IsSprinting())
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bIsSprinting = false;
	}
}

void ABGPlayer::Interaction()
{
	if (CurrentInteractionActorInView)
	{
		CurrentInteractionActorInView->OnInteraction(this);
		CurrentInteractionActorInView = nullptr;
	}
}

void ABGPlayer::Attack()
{
	if (CurrentWeapon)
	{
		if (!IsReloading() && !IsSwitchingWeapon())
		{
			if (IsSprinting())
			{
				SprintRelease();
			}

			CurrentWeapon->FireStart();
			//UE_LOG(LogClass, Warning, TEXT("Attack"));
			//AnimInstance->PlayFireMontage();
		}
	}
}

void ABGPlayer::AttackEnd()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->FireEnd();
	}
}

void ABGPlayer::ToggleCrouch()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		if (GetCharacterMovement()->IsCrouching())
		{
			ACharacter::UnCrouch();
		}
		else
		{
			ACharacter::Crouch();
		}
	}
}

void ABGPlayer::DoJump()
{
	if (!IsReloading() && !IsSwitchingWeapon())
	{
		ACharacter::Jump();
	}
}

void ABGPlayer::Reload()
{
	if (!GetCharacterMovement()->IsFalling() && !IsSwitchingWeapon())
	{
		if (CurrentWeapon && CurrentWeapon->IsCanReload())
		{
			if (IsSprinting())
			{
				SprintRelease();
			}

			if (IsAiming())
			{
				AimRelease();
			}

			SetIsReloading(true);
			AnimInstance->PlayMontage(TEXT("ReloadRifle"));
		}
	}
}

void ABGPlayer::DropWeapon()
{
	if (CurrentWeapon)
	{
		UE_LOG(LogClass, Warning, TEXT("Drop"));
		FVector CamLoc;
		FRotator CamRot;
		BGPlayerController->GetPlayerViewPoint(CamLoc, CamRot);

		FVector SpawnLocation;
		FRotator SpawnRotation = CamRot;

		const FVector Direction = CamRot.Vector();
		const FVector TraceStart = GetActorLocation();
		const FVector TraceEnd = TraceStart + (Direction * 500.f);

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = false;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActor(this);
		
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

		if (Hit.bBlockingHit)
		{
			SpawnLocation = Hit.ImpactPoint + (Hit.ImpactNormal * 20.f);
		}
		else
		{
			SpawnLocation = TraceEnd;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABGWeaponPickup* NewWeaponPickup = GetWorld()->SpawnActor<ABGWeaponPickup>(CurrentWeapon->PickupClass);
		if (NewWeaponPickup)
		{
			UStaticMeshComponent* MeshComp = NewWeaponPickup->GetMesh();
			if (MeshComp)
			{
				auto BGGameInstance = Cast<UBGGameInstance>(GetGameInstance());
				if (BGGameInstance)
				{
					// Create Pickup
					auto PickupMesh = BGGameInstance->GetStaticMesh(CurrentWeapon->GetWeaponName());
					if (PickupMesh)
					{
						MeshComp->SetStaticMesh(PickupMesh);
					}
					else
					{
						UE_LOG(LogClass, Error, TEXT("Pickup StaticMesh not exist...!!"));
						return;
					}
					MeshComp->SetSimulatePhysics(true);
					MeshComp->AddTorqueInRadians(FVector(1.f, 1.f, 1.f) * 400000.f);

					//Update Weapon state
					NewWeaponPickup->SetWeapon(CurrentWeapon);
					ItemStatusComponent->RemoveWeapon(CurrentWeapon, false);
					
					//Update Widget
					BGPlayerController->GetBGHUD()->GetUserWidget()->BindWeaponInfo(nullptr);
					BGPlayerController->GetBGHUD()->GetInventoryWidget()->
						GetWeaponInvnetoryWidget(CurrentWeapon->GetWeaponInventoryIndex())->ClearWidget();

					AnimInstance->SetIsEquipWeapon(false);
					
					CurrentWeapon = nullptr;
					
					UE_LOG(LogClass, Warning, TEXT("Spawn pickup success."));
				}
				else
				{
					UE_LOG(LogClass, Error, TEXT("GameInstance not exist...!!"));
					return;
				}
			}
		}

	}

}

//void ABGPlayer::SwitchWeapon()
//{
//	if (PreviousWeapon && !IsReloading() && !GetCharacterMovement()->IsFalling())
//	{
//		if (IsSprinting())
//		{
//			SprintRelease();
//		}
//
//		if (IsAiming())
//		{
//			AimRelease();
//		}
//
//		AttackEnd();
//
//		UE_LOG(LogClass, Warning, TEXT("Switch"));
//		SetIsSwitchingWeapon(true);
//		AnimInstance->PlayMontage(TEXT("SwitchWeapon"));
//	}
//
//}

void ABGPlayer::Slot1()
{
	UE_LOG(LogClass, Warning, TEXT("Switching Slot1 start."));
	if (nullptr != ItemStatusComponent->GetWeapon(0))
	{
		AnimInstance->SetIsEquipWeapon(true);
		if (ItemStatusComponent->GetCurrentWeaponIndex() != 0)
		{
			UE_LOG(LogClass, Warning, TEXT("Switching Slot1"));
			if (IsSprinting())
			{
				SprintRelease();
			}

			if (IsAiming())
			{
				AimRelease();
			}
			
			PreviousWeapon = CurrentWeapon;

			CurrentWeapon = ItemStatusComponent->GetWeapon(0);
			ItemStatusComponent->SetCurrentWeaponIndex(0);
			//CurrentWeaponIndex = 0;
			//CurrentWeapon = WeaponInventory[0];

			if (BGPlayerController)
			{
				BGPlayerController->GetBGHUD()->GetUserWidget()->BindWeaponInfo(CurrentWeapon);
			}

			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainWeaponSocket);
			if (PreviousWeapon)
			{
				PreviousWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SecondWeaponSocket);
			}
			AnimInstance->PlayMontage(TEXT("SwitchWeapon"));
			SetIsSwitchingWeapon(true);
		}
	}
}

void ABGPlayer::Slot2()
{
	UE_LOG(LogClass, Warning, TEXT("Switching Slot2 start."));
	if (nullptr != ItemStatusComponent->GetWeapon(1))
	{
		AnimInstance->SetIsEquipWeapon(true);
		if (ItemStatusComponent->GetCurrentWeaponIndex() != 1)
		{
			UE_LOG(LogClass, Warning, TEXT("Switching Slot2"));
			if (IsSprinting())
			{
				SprintRelease();
			}
			
			if (IsAiming())
			{
				AimRelease();
			}

			PreviousWeapon = CurrentWeapon;

			CurrentWeapon = ItemStatusComponent->GetWeapon(1);
			ItemStatusComponent->SetCurrentWeaponIndex(1);
			//CurrentWeaponIndex = 1;

			if (BGPlayerController)
			{
				BGPlayerController->GetBGHUD()->GetUserWidget()->BindWeaponInfo(CurrentWeapon);
			}

			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainWeaponSocket);
			if (PreviousWeapon)
			{
				PreviousWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SecondWeaponSocket);
			}
			AnimInstance->PlayMontage(TEXT("SwitchWeapon"));
			SetIsSwitchingWeapon(true);
		}
	}
}
//
//void ABGPlayer::Slot3()
//{
//	if (WeaponInventory[2])
//	{
//		if (CurrentWeaponIndex != 2)
//		{
//			if (CurrentWeapon)
//			{
//				PreviousWeapon = CurrentWeapon;
//			}
//			CurrentWeapon = WeaponInventory[2];
//			AnimInstance->PlayMontage(TEXT("SwitchWeapon"));
//		}
//	}
//}
//
//void ABGPlayer::Slot4()
//{
//	if (WeaponInventory[3])
//	{
//		if (CurrentWeaponIndex != 3)
//		{
//			if (CurrentWeapon)
//			{
//				PreviousWeapon = CurrentWeapon;
//			}
//			CurrentWeapon = WeaponInventory[3];
//			AnimInstance->PlayMontage(TEXT("SwitchWeapon"));
//		}
//	}
//}
//
//void ABGPlayer::Slot5()
//{
//	if (WeaponInventory[4])
//	{
//		if (CurrentWeaponIndex != 4)
//		{
//			if (CurrentWeapon)
//			{
//				PreviousWeapon = CurrentWeapon;
//			}
//			CurrentWeapon = WeaponInventory[4];
//			AnimInstance->PlayMontage(TEXT("SwitchWeapon"));
//		}
//	}
//}

void ABGPlayer::ToggleInventory()
{
	UE_LOG(LogClass, Warning, TEXT("ToggleInventory"));
	auto HUD = BGPlayerController->GetBGHUD();
	if (HUD->IsInventoryOnScreen())
	{
		HUD->RemoveInventoryWidgetOnScreen();
		BGPlayerController->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		HUD->DrawInventoryWidgetOnScreen();
		BGPlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void ABGPlayer::DamageSelf()
{
	FDamageEvent DamageEvent;
	TakeDamage(10.f, DamageEvent, GetController(), this);
}

void ABGPlayer::ShowMeTheMoney()
{
	ItemStatusComponent->AddMoney(500);
}

void ABGPlayer::ChangeViewMode(EViewMode NewMode)
{
	if (NewMode == CurrentViewMode)
	{
		return;
	}
	
	switch (NewMode)
	{
	case EViewMode::VIEW_TPS:
	{
		bUseControllerRotationYaw = false;
		BGPlayerController->SetControlRotation(DirectionVector.Rotation());
		//GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

		if (CurrentViewMode == EViewMode::VIEW_FPS)
		{
			Camera->SetActive(true);
			ScopeCamera->SetActive(false);
			GetMesh()->bOwnerNoSee = false;
			GetMesh()->MarkRenderStateDirty();
		}

		break;
	}
	case EViewMode::VIEW_FREE:
	{
		bUseControllerRotationYaw = false;
		//GetCharacterMovement()->bOrientRotationToMovement = false;
		break;
	}
	case EViewMode::VIEW_FPS:
	{
		bUseControllerRotationYaw = true;
		Camera->SetActive(false);
		ScopeCamera->SetActive(true);
		GetMesh()->bOwnerNoSee = true;
		GetMesh()->MarkRenderStateDirty();
		break;
	}
	}
	
	CurrentViewMode = NewMode;

}


void ABGPlayer::ReloadingWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
		SetIsReloading(false);
	}
}

void ABGPlayer::RemoveWeapon(ABGWeapon * Weapon, bool bDestroy)
{
	// If Weapon valid pointer
	if (Weapon)
	{
		//// If Weapon in WeaponInventory
		//if (WeaponInventory.Contains(Weapon))
		//{
		//	int32 Index = WeaponInventory.Find(Weapon);
		//	WeaponInventory[Index] = nullptr;
		//	CurrentWeapon = nullptr;
		//	UE_LOG(LogClass, Warning, TEXT("Remove weapon in %d."), Index);
		//}
		//if (bDestroy)
		//{
		//	Weapon->Destroy();
		//}
		//else
		//{
		//	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//	Weapon->SetActorEnableCollision(ECollisionEnabled::NoCollision);
		//	Weapon->SetActorHiddenInGame(true);
		//}
	}

}

//void ABGPlayer::DoSwitching()
//{
//	UE_LOG(LogClass, Warning, TEXT("DoSwitching"));
//	auto Temp = CurrentWeapon;
//	CurrentWeapon = PreviousWeapon;
//	PreviousWeapon = Temp;
//
//	if (BGPlayerController)
//	{
//		BGPlayerController->GetHUDWidget()->BindWeaponInfo(CurrentWeapon);
//	}
//
//	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainWeaponSocket);
//	PreviousWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SecondWeaponSocket);
//}

ABGInteractionActor* ABGPlayer::GetInteractionActorInView()
{
	FVector CameraVector;
	FRotator CameraRotator;

	GetController()->GetPlayerViewPoint(CameraVector, CameraRotator);
	const FVector TraceStart = CameraVector;
	const FVector Direction = CameraRotator.Vector();
	const FVector TraceEnd = TraceStart + (Direction * 600.f);

	FCollisionQueryParams TraceParams(TEXT("TracePickupActor"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = false;

	FHitResult HitResult;

	bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	return Cast<ABGInteractionActor>(HitResult.GetActor());
}

ABGWeapon * const ABGPlayer::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

