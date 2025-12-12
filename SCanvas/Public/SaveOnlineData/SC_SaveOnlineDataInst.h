// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sc_PrimaryProductData.h"
#include "Sc_ProductActorBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SC_SaveOnlineDataInst.generated.h"

/**
 * 
 */

class USc_PrimaryProductData;



USTRUCT(Blueprintable, BlueprintType)
struct FOnlineSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FTransform ActorTransform;

	UPROPERTY(BlueprintReadWrite)
	USc_PrimaryProductData* ProductData = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FVariationData VariationData;

	UPROPERTY(BlueprintReadWrite)
	TArray<FLightData> LightData;
	
};

USTRUCT(Blueprintable, BlueprintType)
struct FOnlineSaveDataString
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString ActorTransform;

	UPROPERTY(BlueprintReadWrite)
	FString ProductData;

	UPROPERTY(BlueprintReadWrite)
	FString VariationData;

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> LightData;
	
};

UCLASS()
class SCANVAS_API USC_SaveOnlineDataInst : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	TArray<FOnlineSaveDataString> OnlineSaveDataString;
	
	UPROPERTY()
	TArray<FOnlineSaveData> OnlineSaveDataList;
	
	/*
	UFUNCTION(BlueprintCallable)
	void SaveDataOnline();

	UFUNCTION(BlueprintCallable)
	void LoadOnlineData();
	*/

	UFUNCTION(BlueprintCallable)
	static bool SCGetFieldValue(FString& Value, const FString& JsonString, const FString& Field);
	
	UFUNCTION(BlueprintCallable)
	FString GetLevelDataAsJson();

	//This will collect only Light System V2
	UFUNCTION(BlueprintCallable)
	FString GetLevelDataAsJsonV2();
	
	UFUNCTION(BlueprintCallable)
	bool LoadLevelFromJsonData(const FString& JsonString);


	
};
