// Fill out your copyright notice in the Description page of Project Settings.

#include "BGWeapon.h"
#include "BGPlayer.h"
#include "BGProjectile.h"
#include "BGPlayerController.h"
#include "BGPlayerAnimInstance.h"
#include "BGWeaponStatComponent.h"
#include "BGGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABGWeapon::ABGWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	//	SK_WEAPON(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	//if (SK_WEAPON.Succeeded())
	//{
	//	WeaponMesh->SetSkeletalMesh(SK_WEAPON.Object);
	//	MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("muzzle"));
	//}
	
	//WeaponStat = CreateDefaultSubobject<UBGWeaponStatComponent>(TEXT("WeaponStat"));

	//WeaponStat.Damage = 10.f;
	//WeaponStat.FireDelay= 0.1f;
	//WeaponStat.MaxAmmo = 30;
	//WeaponStat.WeaponName = TEXT("M4A1");
	//WeaponStat.WeaponType = EWeaponType::AR;
	ItemType = EItemType::WEAPON;

	FireTimer = 0.f;
	CurrentAmmo = 0;
	bWantsToFire = false;
	//bNeedToReload = true;

	ProjectileClass = ABGProjectile::StaticClass();

}

// Called when the game starts or when spawned
void ABGWeapon::BeginPlay()
{
	Super::BeginPlay();

	//if (BGGameInstance)
	//{
	//	if (BGGameInstance->GetAssetPath(WeaponStat.WeaponName, WeaponAssetToLoad))
	//	{
	//		UE_LOG(LogClass, Error, TEXT("Asset exist!!!"));
	//		AssetStreamingHandle = BGGameInstance->StreamableManager.RequestAsyncLoad(WeaponAssetToLoad,
	//			FStreamableDelegate::CreateUObject(this, &ABGWeapon::OnAssetLoadCompleted));
	//	}
	//	else
	//	{
	//		UE_LOG(LogClass, Error, TEXT("Asset not exist!!!"));
	//	}
	//}

}

// Called every frame
void ABGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bWantsToFire)
	{
		if (ProjectileClass)
		{
			FireTimer += DeltaTime;
			
			if (FireTimer > FireDelay)
			{
				FireTimer = 0.f;
				Fire();
			}
		}
	}
	else
	{
		if (FireTimer != 0.f)
		{
			FireTimer = 0.f;
		}
	}

}

void ABGWeapon::FireStart()
{
	UE_LOG(LogClass, Warning, TEXT("Fire!!"));
	bWantsToFire = true;
	Fire();
}

void ABGWeapon::FireEnd()
{
	UE_LOG(LogClass, Warning, TEXT("Fire End."));
	bWantsToFire = false;
}

void ABGWeapon::Reload()
{
	if (CurrentAmmo < MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
		OnCurrentAmmoVarying.Execute();
	}
}

void ABGWeapon::InitItemData(FBGItemData* NewItemData)
{
	if (NewItemData)
	{
		//UE_LOG(LogClass, Error, TEXT("%s"), *NewItemData->StaticStruct()->GetName());
		// Cast<>가 동작을 안함..
		// UClass가 아니라서 동작을 안하는듯.. 일단 static_cast<>로 대체함.
		//TODO : 나중에 Custom Cast를 구현해서 대체할 것.
		FBGWeaponData* WeaponData = static_cast<FBGWeaponData*>(NewItemData);
		if (WeaponData)
		{
			//Check weapon type
			auto NewWeaponType = GetWeaponTypeFromString(WeaponData->WeaponType);
			if (NewWeaponType == EWeaponType::INVALID)
			{
				UE_LOG(LogClass, Error, TEXT("Invalid WeaponType.."));
				return;
			}

			Super::InitItemData(NewItemData);

			WeaponType = NewWeaponType;
			Damage = WeaponData->Damage;
			FireDelay = WeaponData->FireDelay;
			MaxAmmo = WeaponData->MaxAmmo;

			auto BGGameInstance = Cast<UBGGameInstance>(GetGameInstance());
			if (BGGameInstance)
			{
				auto SkeletalMesh = BGGameInstance->GetSkeletalMesh(ItemName);
				if (SkeletalMesh)
				{
					WeaponMesh->SetSkeletalMesh(SkeletalMesh);
				}

			}
		}
		else
		{
			UE_LOG(LogClass, Error, TEXT("Invalid data type in ABGWeapon::InitItemData"));
		}



	}

}

void ABGWeapon::Dropped()
{
	

}

//void ABGWeapon::SetWeaponOwner(ABGPlayer * Player)
//{
//	WeaponOwner = Player;
//}

bool ABGWeapon::IsCanReload() const
{
	return CurrentAmmo < MaxAmmo;
}

bool ABGWeapon::IsNeedToReload() const
{
	return CurrentAmmo <= 0;
}

int32 ABGWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

EWeaponType ABGWeapon::GetWeaponType() const
{
	return WeaponType;
}

const FString & ABGWeapon::GetWeaponName() const
{
	return ItemName;
}

FHitResult ABGWeapon::WeaponTrace(const FVector & Start, const FVector & End)
{
	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult HitResult(ForceInit);
	GetWorld()->LineTraceSingleByProfile(HitResult, Start, End, TEXT("WeaponTrace"), TraceParams);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f);

	return HitResult;
}

void ABGWeapon::Fire()
{
	if (IsNeedToReload())
	{
		UE_LOG(LogClass, Warning, TEXT("Need to reload!!"));
		return;
	}

	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName(TEXT("muzzle")));
	UWorld* World = GetWorld();

	if (World)
	{
		auto PlayerController = Cast<ABGPlayerController>(ItemOwner->GetController());
		FVector LaunchDirection = FVector::ZeroVector;

		if (PlayerController)
		{
			FVector CamLoc;
			FRotator CamRot;
			PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
			
			FVector StartLoc = CamLoc;
			FVector EndLoc = StartLoc + 10000.f * CamRot.Vector();
			FHitResult Hit = WeaponTrace(StartLoc, EndLoc);

			if (Hit.bBlockingHit)
			{
				LaunchDirection = Hit.ImpactPoint - MuzzleLocation;
			}
			else
			{
				LaunchDirection = Hit.TraceEnd - MuzzleLocation;
			}

		}


		FActorSpawnParameters SpawnParmas;
		SpawnParmas.Owner = this;
		SpawnParmas.Instigator = ItemOwner;
		ABGProjectile* Projectile = World->SpawnActor<ABGProjectile>(ProjectileClass, MuzzleLocation, LaunchDirection.Rotation(), SpawnParmas);
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
		DrawDebugLine(World, MuzzleLocation, MuzzleLocation + LaunchDirection * 1000.f, FColor::Red, false, 0.5f);

		auto PlayerAnimInstance = Cast<UBGPlayerAnimInstance>(ItemOwner->GetMesh()->GetAnimInstance());
		if (PlayerAnimInstance)
		{
			PlayerAnimInstance->PlayFireMontage();
		}

		--CurrentAmmo;

		OnWeaponFire.Broadcast(); 

	}

}

FVector ABGWeapon::GetAdjustAim()
{
	auto PlayerController = Instigator ? Cast<ABGPlayerController>(Instigator->GetController()) : nullptr;
	FVector FinalAim = FVector::ZeroVector;
	if (PlayerController)
	{
		FVector CamLoc;
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}

	return FinalAim;
}

void ABGWeapon::OnAssetLoadCompleted()
{
	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(WeaponAssetToLoad);
	if (LoadedAssetPath.IsValid())
	{
		WeaponMesh->SetSkeletalMesh(LoadedAssetPath.Get());
	}
}

