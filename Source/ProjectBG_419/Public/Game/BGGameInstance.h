// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "BGGameInstance.generated.h"

/**
 * 
 */


UCLASS(config=Game)
class PROJECTBG_419_API UBGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBGGameInstance();
	
protected:
	virtual void Init() override;
	
public:
	bool GetAssetPath(const FString& AssetName, FSoftObjectPath& AssetPath);
	FBGWeaponData* const GetWeaponDataByName(const FString& WeaponName);
	FBGWeaponData* const GetWeaponDataByRowNumber(int32 RowNumber);


public:
	class USkeletalMesh* const GetSkeletalMesh(const FString& AssetName);
	class UStaticMesh* const GetStaticMesh(const FString& AssetName);
	FORCEINLINE int32 GetSkeletalMeshTableSize() const { return WeaponSKTable.Num(); }
	FORCEINLINE int32 GetStaticMeshTableSize() const { return WeaponSMTable.Num(); }

public:
	FStreamableManager StreamableManager;

private:
	void InitMeshTable();
	
private:
	//UPROPERTY(config)
	//TMap<FString, FSoftObjectPath> WeaponSkeletalMeshPathMap;

	UPROPERTY()
	TMap<FString, class USkeletalMesh*> WeaponSKTable;

	UPROPERTY()
	TMap<FString, class UStaticMesh*> WeaponSMTable;

	UPROPERTY()
	class UDataTable* WeaponDataTable;

};
