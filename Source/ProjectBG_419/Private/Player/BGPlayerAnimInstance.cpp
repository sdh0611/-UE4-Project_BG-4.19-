// Fill out your copyright notice in the Description page of Project Settings.

#include "BGPlayerAnimInstance.h"
#include "BGPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"

UBGPlayerAnimInstance::UBGPlayerAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	CurrentPawnDirection = 0.f;
	bIsInAir = false;
	bIsAiming= false;
	bIsCrouching = false;
	bIsEquipWeapon = false;
	fAimPitch = 0.f;
	fAimYaw = 0.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		SHOTGUN_Aim(TEXT("AnimMontage'/Game/Animation/Fire_Shotgun_Hip_Montage.Fire_Shotgun_Hip_Montage'"));
	if (SHOTGUN_Aim.Succeeded())
	{
		//FireMontageShotgunAim = SHOTGUN_Aim.Object;
		MontageTable.Add(TEXT("FireShotgunHip"), SHOTGUN_Aim.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		RIFLE_Aim(TEXT("AnimMontage'/Game/Animation/Fire_Rifle_Ironsights_Montage.Fire_Rifle_Ironsights_Montage'"));
	if (RIFLE_Aim.Succeeded())
	{
		//FireMontageRifleAim= RIFLE_Aim.Object;
		MontageTable.Add(TEXT("FireRifleAim"), RIFLE_Aim.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		RIFLE_Hip(TEXT("AnimMontage'/Game/Animation/Fire_Rifle_Hip_Montage.Fire_Rifle_Hip_Montage'"));
	if (RIFLE_Hip.Succeeded())
	{
		//FireMontageRifle = RIFLE_Hip.Object;
		MontageTable.Add(TEXT("FireRifleHip"), RIFLE_Hip.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		RIFLE_Reload(TEXT("AnimMontage'/Game/Animation/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'"));
	if (RIFLE_Reload.Succeeded())
	{
		//FireMontageRifle = RIFLE_Hip.Object;
		MontageTable.Add(TEXT("ReloadRifle"), RIFLE_Reload.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONTAGE_Switch(TEXT("AnimMontage'/Game/Animation/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'"));
	if (MONTAGE_Switch.Succeeded())
	{
		//FireMontageRifle = RIFLE_Hip.Object;
		MontageTable.Add(TEXT("SwitchWeapon"), MONTAGE_Switch.Object);
	}


}

void UBGPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		CurrentPawnSpeed = Speed.Size();
		
		CurrentPawnDirection = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
				
		const FRotator Delta = Pawn->GetControlRotation() - Pawn->GetActorRotation();
		FRotator Result = FMath::RInterpTo(FRotator(fAimPitch, fAimYaw, 0.f), Delta, DeltaSeconds, 15.f);
		fAimPitch = FMath::ClampAngle(Result.Pitch, -90.f, 90.f);
		fAimYaw = FMath::ClampAngle(Result.Yaw, -90.f, 90.f);


		auto Player = Cast<ABGPlayer>(Pawn);
		if (Player)
		{
			bIsInAir = Player->GetMovementComponent()->IsFalling();
			bIsCrouching = Player->GetMovementComponent()->IsCrouching();
		}

	}

}

void UBGPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UBGPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UBGPlayerAnimInstance::SetIsAiming(bool NewState)
{
	bIsAiming = NewState;
}

void UBGPlayerAnimInstance::SetIsEquipWeapon(bool NewState)
{
	bIsEquipWeapon = NewState;
}

void UBGPlayerAnimInstance::PlayFireMontage()
{
	//auto Pawn = TryGetPawnOwner();
	//if (::IsValid(Pawn))
	//{
	//	auto Player = Cast<ABGPlayer>(Pawn);
	//	if (Player)
	//	{
	//		if (Player->IsAiming())
	//		{
	//			bIsAiming = true;
	//		}
	//	}
	//
	//}

	if (!Montage_IsPlaying(CurrentPlayMontage))
	{
		if (bIsAiming)
		{
			SetCurrentMontage(TEXT("FireRifleAim"));
		}
		else
		{
			SetCurrentMontage(TEXT("FireRifleHip"));
			//CurrentPlayMontage = FireMontageShotgunAim;
		}
		
		Montage_Play(CurrentPlayMontage);
	}

	//if (!Montage_IsPlaying(FireMontageRifle))
	//{
	//	Montage_Play(FireMontageRifle);
	//}
}

void UBGPlayerAnimInstance::PlayMontage(const FString & MontageName)
{
	if (MontageTable.Contains(MontageName))
	{
		UE_LOG(LogClass, Warning, TEXT("Contain Montage"));
		UAnimMontage* PlayMontage = MontageTable[MontageName];
		//if (!Montage_IsPlaying(PlayMontage))
		//{
			Montage_Play(PlayMontage);
			SetCurrentMontage(MontageName);
		//}
	}

}

void UBGPlayerAnimInstance::SetCurrentMontage(const FString & MontageName)
{
	if (MontageTable.Contains(MontageName))
	{
		if (CurrentPlayMontage != MontageTable[MontageName])
		{
			CurrentPlayMontage = MontageTable[MontageName];
		}
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Montage not exist in MontageTable."));
	}
}

bool UBGPlayerAnimInstance::IsMontagePlaying(const FString& MontageName)
{
	bool bResult = false;

	if (MontageTable.Contains(MontageName))
	{
		bResult = Montage_IsPlaying(MontageTable[MontageName]);
	}

	return bResult;
}

void UBGPlayerAnimInstance::AnimNotify_ReloadEndCheck()
{
	//UE_LOG(LogClass, Warning, TEXT("ReloadEnd"));
	OnReloadEnd.Execute();
}

void UBGPlayerAnimInstance::AnimNotify_SwitchWeapon()
{
	UE_LOG(LogClass, Warning, TEXT("Switch Notify"));
	//OnSwitch.Execute();
}

void UBGPlayerAnimInstance::AnimNotify_SwitchWeaponEnd()
{
	auto Player = Cast<ABGPlayer>(TryGetPawnOwner());
	if (Player)
	{
		Player->SetIsSwitchingWeapon(false);
	}
}

