// Fill out your copyright notice in the Description page of Project Settings.

#include "BGGameInstance.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"


UBGGameInstance::UBGGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_BGWeaponData(TEXT("DataTable'/Game/GameData/WeaponData2.WeaponData2'"));
	if (DT_BGWeaponData.Succeeded())
	{
		WeaponDataTable = DT_BGWeaponData.Object;
		InitMeshTable();
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("DataTable path is invalid."));
	}
	

}

void UBGGameInstance::Init()
{
	Super::Init();

	//FString Name = GetWeaponData(FName(TEXT("AK-47")))->WeaponName;
	
	auto Data = GetWeaponDataByName(TEXT("AK-47"));

	if (Data)
	{
		UE_LOG(LogClass, Warning, TEXT("Name : %s, Damage : %f"), *Data->ItemName, Data->Damage);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("WeaponData not exist."));
	}
}

bool UBGGameInstance::GetAssetPath(const FString & AssetName, FSoftObjectPath& AssetPath) 
{
	//if (WeaponSkeletalMeshPathMap.Contains(AssetName))
	//{
	//	AssetPath = WeaponSkeletalMeshPathMap[AssetName];
	//	return true;
	//}

	return false;
}

FBGWeaponData * const UBGGameInstance::GetWeaponDataByName(const FString& WeaponName)
{
	TArray<FName> Names = WeaponDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = WeaponDataTable->FindRow<FBGWeaponData>(Name, TEXT(""));
		if (Data->ItemName.Compare(WeaponName) == 0)
		{
			return Data;
		}
	}

	return nullptr;
}

FBGWeaponData * const UBGGameInstance::GetWeaponDataByRowNumber(int32 RowNumber)
{
	FName Row(*FString::FromInt(RowNumber));
	auto Data = WeaponDataTable->FindRow<FBGWeaponData>(Row, TEXT(""));
	if (Data)
	{
		return Data;
	}

	return  nullptr;
}

FBGRecoveryData * const UBGGameInstance::GetRecoveryDataByName(const FString & RecoveryName)
{
	TArray<FName> Names = RecoveryDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = RecoveryDataTable->FindRow<FBGRecoveryData>(Name, TEXT(""));
		if (Data->ItemName.Compare(RecoveryName) == 0)
		{
			return Data;
		}
	}

	return nullptr;
}

USkeletalMesh * const UBGGameInstance::GetSkeletalMesh(const FString & AssetName)
{
	if (WeaponSKTable.Contains(AssetName))
	{
		return WeaponSKTable[AssetName];
	}

	return nullptr;
}

UStaticMesh * const UBGGameInstance::GetStaticMesh(const FString & AssetName)
{
	if (WeaponSMTable.Contains(AssetName))
	{
		return WeaponSMTable[AssetName];
	}

	return nullptr;
}

//Use in constructor
void UBGGameInstance::InitMeshTable()
{
	TArray<FName> Names = WeaponDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = WeaponDataTable->FindRow<FBGWeaponData>(Name, TEXT(""));
		//Init SkeletalMesh table.
		ConstructorHelpers::FObjectFinder<USkeletalMesh>
			SK_Object(*(Data->SkeletalMeshPath));
		if (SK_Object.Succeeded())
		{
			WeaponSKTable.Add(Data->ItemName, SK_Object.Object);
		}

		//Init StaticMesh table.
		ConstructorHelpers::FObjectFinder<UStaticMesh>
			SM_Object(*(Data->StaticMeshPath));
		if (SM_Object.Succeeded())
		{
			WeaponSMTable.Add(Data->ItemName, SM_Object.Object);
		}

	}

}
