// Fill out your copyright notice in the Description page of Project Settings.

#include "BGWeaponInventoryWidget.h"
#include "BGWeapon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UBGWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UImage* NewImage = Cast<UImage>(GetWidgetFromName(TEXT("WeaponImage")));
	if (NewImage)
	{
		WeaponImage = NewImage;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Cannot find WeaponImage.."));
	}

	UTextBlock* NewWeaponName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtWeaponName")));
	if (NewWeaponName)
	{
		WeaponName = NewWeaponName;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Cannot find WeaponName.."));
	}

	UTextBlock* NewCurrentAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentAmmo")));
	if (NewCurrentAmmoText)
	{
		CurrentAmmoText = NewCurrentAmmoText;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Cannot find CurrentAmmoText.."));
	}

	UTextBlock* NewTotalAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalAmmo")));
	if (NewTotalAmmoText)
	{
		TotalAmmoText = NewTotalAmmoText;
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("Cannot find TotalAmmoText.."));
	}

}

void UBGWeaponInventoryWidget::BindWeapon(ABGWeapon * NewWeapon)
{
	if (NewWeapon)
	{
		Weapon = NewWeapon;
		//Set weapon image
		// 일단 임시로 nullptr로 초기화.
		UTexture2D* Image = nullptr;
		WeaponImage->SetBrushFromTexture(Image);

		//Set weapon name
		FText Name = FText::FromString(Weapon->GetWeaponName());
		WeaponName->SetText(Name);

		//Set current ammo
		FText CurrentAmmo = FText::FromString(FString::FromInt(Weapon->GetCurrentAmmo()));
		CurrentAmmoText->SetText(CurrentAmmo);

		//일단 임시로 MaxAmmo값으로 초기화.
		// TODO : ItemStatusComponent의 탄창들의 값으로 초기화 시켜줄 것.
		FText TotalAmmo = FText::FromString(FString::FromInt(Weapon->GetMaxAmmo()));
		TotalAmmoText->SetText(TotalAmmo);
		Weapon->OnWeaponFire.AddUObject(this, &UBGWeaponInventoryWidget::UpdateCurrentAmmoText);
		Weapon->OnCurrentAmmoVarying.AddUObject(this, &UBGWeaponInventoryWidget::UpdateCurrentAmmoText);
	}

}

void UBGWeaponInventoryWidget::UpdateCurrentAmmoText()
{
	if (Weapon)
	{
		FText CurrentAmmo = FText::FromString(FString::FromInt(Weapon->GetCurrentAmmo()));
		CurrentAmmoText->SetText(CurrentAmmo);
	}
}

void UBGWeaponInventoryWidget::UpdateInfo()
{
	if (Weapon)
	{
		
	}
	else
	{

	}

}

void UBGWeaponInventoryWidget::ClearWidget()
{
	Weapon = nullptr;
	WeaponImage->SetBrushFromTexture(nullptr);
	WeaponName->SetText(FText::FromString((TEXT(""))));
	CurrentAmmoText->SetText(FText::FromString((TEXT(""))));
	TotalAmmoText->SetText(FText::FromString((TEXT(""))));
}
