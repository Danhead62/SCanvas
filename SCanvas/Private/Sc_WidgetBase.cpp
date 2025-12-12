// Fill out your copyright notice in the Description page of Project Settings.


#include "Sc_WidgetBase.h"

#include "Sc_PrimaryProductData.h"
#include "Components/Slider.h"
#include "Engine/AssetManager.h"


void USc_WidgetBase::PrimaryAssetLoaded()
{
	if (USc_PrimaryProductData* LoadedAsset = Cast<USc_PrimaryProductData>(StreamableHandle.Get()->GetLoadedAsset()))
	{
		EventPrimaryAssetLoaded(LoadedAsset);
	}
	
}

void USc_WidgetBase::AsyncLoadCancel(const FSoftObjectPath SoftPath)
{
	TArray<FSoftObjectPath> RequestedPaths;
	StreamableHandle->GetRequestedAssets(RequestedPaths);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("Progress: %i"), StreamableHandle.GetSharedReferenceCount()));
	for (FSoftObjectPath RequestedPath : RequestedPaths)
	{
		TArray<TSharedRef<FStreamableHandle>> HandleList;
		StreamableHandle->GetOwningManager()->GetActiveHandles(RequestedPath, HandleList);
		for (TSharedRef<FStreamableHandle> Handle : HandleList)
		{
			Handle->CancelHandle();
		} 
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("Progress: %i"), StreamableHandle->GetPriority()));
		//StreamableHandle->GetOwningManager()->Unload(RequestedPath);
	}
	
	//StreamableHandle->CancelHandle();
	//StreamableHandle->ReleaseHandle();
	
}

void USc_WidgetBase::UnloadPrimaryAssets(const TArray<FPrimaryAssetId>& AssetsToUnload)
{
	UAssetManager::Get().UnloadPrimaryAssets(AssetsToUnload);
}

void USc_WidgetBase::AsyncLoadPrimaryAsset(const FSoftObjectPath SoftObjectPath)
{
	MainSoftObjectPath = SoftObjectPath;
	StreamableManager = &UAssetManager::Get().GetStreamableManager();
	TArray<FSoftObjectPath> SoftPath;
	SoftPath.Add(SoftObjectPath);
	StreamableHandle = StreamableManager->RequestAsyncLoad(SoftPath, FStreamableDelegate::CreateUObject(this, &USc_WidgetBase::PrimaryAssetLoaded),FStreamableManager::AsyncLoadHighPriority,true);
	
}
