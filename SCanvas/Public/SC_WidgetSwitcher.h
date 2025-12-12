// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetSwitcher.h"
#include "SC_WidgetSwitcher.generated.h"

/**
 * 
 */
UCLASS()
class SCANVAS_API USC_WidgetSwitcher : public UWidgetSwitcher
{
	GENERATED_BODY()

	public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetSwitched, int32, ActivatedIndex);
	virtual void SetActiveWidgetIndex(int32 Index) override;

	UPROPERTY(BlueprintAssignable)
	FWidgetSwitched OnWidgetSwitch;
	
};
