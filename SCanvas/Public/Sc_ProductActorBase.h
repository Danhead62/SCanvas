// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_FunctionLibrary.h"
#include "Sc_PrimaryProductData.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Sc_ProductActorBase.generated.h"

//struct FVariationData;
//class USc_PrimaryProductData;
//class UStaticMeshComponent;
//struct FLightPropertyData;

USTRUCT(Blueprintable, BlueprintType)
struct FLightData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName SocketName = "";
	
	UPROPERTY(BlueprintReadWrite)
	float LightIntensity = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float LightTemperature = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float AttentionRadius = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float SourceRadius = 0.0f;

	UPROPERTY(BlueprintReadWrite)
    float SoftSourceRadius = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	bool bCastShadow = false;

	UPROPERTY(BlueprintReadWrite)
	float InnerCone = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float OuterCone = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float IndirectLightingIntensity = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float SourceLength = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	bool bRayTracedShadow = false;
	
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProductDataUpdated);

UCLASS()
class SCANVAS_API ASc_ProductActorBase : public AActor
{
	GENERATED_BODY()

	
protected:
	// Sets default values for this actor's properties
	ASc_ProductActorBase();

	
	virtual void BeginPlay() override;
	
public:	
	
	UFUNCTION(BlueprintCallable, Category="SCanvas")
	void UpdateProduct(USc_PrimaryProductData* ProductDataRef);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="SCanvas")
	UStaticMeshComponent* ProductStaticMesh;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="SCanvas", meta=(ExposeOnSpawn))
	USc_PrimaryProductData* ProductData = nullptr;

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	void UpdateVariation(FVariationData Variation);

	UPROPERTY(BlueprintReadWrite, Category="SCanvas")
	FVariationData VariationData;

	UPROPERTY(BlueprintAssignable, Category="SCanvas")
	FProductDataUpdated OnProductDataUpdated;

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	void UpdateProductFromHistory(USc_PrimaryProductData* ProductDataRef);

	//This is calling from transaction history subsystem in Move and Forward function
	UFUNCTION(BlueprintImplementableEvent)
	void PositionUpdated();

	UFUNCTION(BlueprintPure)
	FVector FindEmptyPosition(AActor* ActorToCheck, float Radius);

	UFUNCTION(BlueprintPure)
	bool FindClosestPointOfArray(TArray<FVector> ArrayToSort, FVector& ClosestEmptyPoint) const;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FLightData> LightDataList;

	UPROPERTY(BlueprintReadOnly)
	TArray<FLightDataV2> LightPropertyList;

	
	//False when spawning product with online data
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	bool bCheckEmptyPosition = true;
	
};
