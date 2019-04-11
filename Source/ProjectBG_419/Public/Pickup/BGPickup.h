// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectBG_419.h"
#include "BGInteractionActor.h"
#include "BGPickup.generated.h"


UCLASS()
class PROJECTBG_419_API ABGPickup : public ABGInteractionActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABGPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteraction(class APawn* Pawn) override;
	virtual void OnFocus();
	virtual void OnFocusEnd();

public:
	void SetActive(bool NewState);

public:
	bool IsActive() const;
	FORCEINLINE class UStaticMeshComponent* GetMesh() const
	{
		return Mesh;
	}

public:
	TSubclassOf<class ABGItem> ItemClass;

protected:
	FBGItemData* ItemData;

protected:
	UPROPERTY(VisibleAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;

private:
	UPROPERTY(VisibleAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = true))
	bool bIsActive;
	
};
