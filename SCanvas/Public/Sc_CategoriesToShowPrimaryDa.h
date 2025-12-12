// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sc_CategoriesToShowPrimaryDa.generated.h"

/**
 * 
 */
UCLASS()
class SCANVAS_API USc_CategoriesToShowPrimaryDa : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SCanvas")
	TArray<FPrimaryAssetType> PrimaryAssetsToShow;
	
};
