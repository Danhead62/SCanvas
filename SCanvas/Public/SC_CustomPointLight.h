// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/PointLight.h"
#include "SC_CustomPointLight.generated.h"

/**
 * 
 */
UCLASS()
class SCANVAS_API ASC_CustomPointLight : public AActor
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	void UpdatePointLight(UPointLightComponent* CopyFrom, UPointLightComponent* CopyTo);
	
};
