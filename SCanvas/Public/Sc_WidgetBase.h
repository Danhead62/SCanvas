// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sc_WidgetBase.generated.h"

struct FStreamableManager;
class USc_PrimaryProductData;
struct FStreamableHandle;
/**
 * 
 */
UCLASS()
class SCANVAS_API USc_WidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	void PrimaryAssetLoaded();

	UFUNCTION(BlueprintImplementableEvent)
	void EventPrimaryAssetLoaded(USc_PrimaryProductData* LoadedData);

	UFUNCTION(BlueprintCallable)
	void AsyncLoadCancel(FSoftObjectPath SoftPath);
	
	UFUNCTION()
	void UnloadPrimaryAssets(const TArray<FPrimaryAssetId>& AssetsToUnload);

	FStreamableManager* StreamableManager;
	TSharedPtr<FStreamableHandle> StreamableHandle = nullptr;
	FSoftObjectPath MainSoftObjectPath;
	
	
	UFUNCTION(BlueprintCallable)
	void AsyncLoadPrimaryAsset(FSoftObjectPath SoftObjectPath);
};
