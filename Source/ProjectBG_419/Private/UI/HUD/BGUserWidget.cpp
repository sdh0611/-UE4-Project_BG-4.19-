// Fill out your copyright notice in the Description page of Project Settings.

#include "BGUserWidget.h"
#include "BGHPComponent.h"
#include "BGWeaponStatComponent.h"
#include "BGPlayerItemStatusComponent.h"
#include "BGWeapon.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"



void UBGUserWidget::BindCharacterHP(UBGHPComponent* HP)
{
	if (HP == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("HPBar is null!"));
		return;
	}

	CurrentCharacterHP = HP;
	CurrentCharacterHP->OnHPChanged.AddUObject(this, &UBGUserWidget::UpdateHP);

}

void UBGUserWidget::BindWeaponInfo(ABGWeapon * Weapon)
{
	if (Weapon == nullptr)
	{
		CurrentAmmoTxt->SetVisibility(ESlateVisibility::Hidden);
		TotalAmmoTxt->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogClass, Error, TEXT("Weapon is null!"));
		return;
	}

	CurrentWeapon = Weapon;
	Weapon->OnWeaponFire.AddUObject(this, &UBGUserWidget::UpdateCurrentAmmo);
	Weapon->OnCurrentAmmoVarying.BindUObject(this, &UBGUserWidget::UpdateCurrentAmmo);
	CurrentAmmoTxt->SetVisibility(ESlateVisibility::Visible);
	TotalAmmoTxt->SetVisibility(ESlateVisibility::Visible);
	UpdateCurrentAmmo();
	//CurrentCharacterHP->OnHPChanged.AddUObject(this, &UBGUserWidget::UpdateHP);

}

void UBGUserWidget::BindPlayerItemStatus(UBGPlayerItemStatusComponent * NewItemStatus)
{
	if (nullptr == NewItemStatus)
	{
		UE_LOG(LogClass, Error, TEXT("ItemStatus is null!"));
		return;
	}

	CurrentPlayerItemStatus = NewItemStatus;
	NewItemStatus->OnMoneyChanged.BindUObject(this, &UBGUserWidget::UpdateItemStatus);
	UpdateItemStatus();
}


//void UBGUserWidget::SetVisibleAmmoInfo(bool NewState)
//{
//	if (NewState)
//	{
//		CurrentAmmoTxt->SetVisibility(ESlateVisibility::Hidden);
//		TotalAmmoTxt->SetVisibility(ESlateVisibility::Hidden);
//	}
//	else
//	{
//		CurrentAmmoTxt->SetVisibility(ESlateVisibility::Visible);
//		TotalAmmoTxt->SetVisibility(ESlateVisibility::Visible);
//	}
//
//}

void UBGUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	if (HPBar == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("HPBar not exist!"));
	}

	DopingBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_DopingBar")));
	if (DopingBar == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("DopingBar not exist!"));
	}

	CurrentAmmoTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentAmmo")));
	if (CurrentAmmoTxt == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("CurrentAmmotxt not exist!"));
	}

	TotalAmmoTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalAmmo")));
	if (TotalAmmoTxt == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("TotalAmmotxt not exist!"));
	}
	CurrentAmmoTxt->SetVisibility(ESlateVisibility::Hidden);
	TotalAmmoTxt->SetVisibility(ESlateVisibility::Hidden);

	CurrentMoneyTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentMoney")));
	if (CurrentMoneyTxt == nullptr)
	{
		UE_LOG(LogClass, Error, TEXT("CurrentMoneytxt not exist!"));
	}

}

void UBGUserWidget::UpdateHP()
{
	if (CurrentCharacterHP.IsValid())
	{
		HPBar->SetPercent(CurrentCharacterHP->GetHPRatio());
	}

}

void UBGUserWidget::UpdateCurrentAmmo()
{
	if (CurrentWeapon.IsValid())
	{
		FText Text;
		auto AmmoText = FString::FromInt(CurrentWeapon->GetCurrentAmmo());
		Text = FText::FromString(AmmoText);

		CurrentAmmoTxt->SetText(Text);
	}
}

void UBGUserWidget::UpdateItemStatus()
{
	if (CurrentPlayerItemStatus.IsValid())
	{
		CurrentMoneyTxt->SetText(FText::FromString(FString::FromInt(CurrentPlayerItemStatus->GetCurrentMoney()).Append(TEXT(" $"))));
	}

}
