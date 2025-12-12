// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sc_PrimaryProductData.generated.h"


class UStaticMesh;
class UTexture2D;

USTRUCT(BlueprintType, Blueprintable)
struct FGoogleSheetDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, DisplayName="Brand Name", VisibleDefaultsOnly)
	FString BrandName;

	UPROPERTY(BlueprintReadOnly, DisplayName="Product Name (English)", VisibleDefaultsOnly)
	FString ProductNameEnglish;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	FString ProductLink;

	//UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, DisplayName="BrandName(JP)")
	//FString BrandNameJp;
	
	//UPROPERTY(BlueprintReadOnly, DisplayName="Product Name (JP)", VisibleDefaultsOnly)
	//FString ProductNameJp;

	UPROPERTY(BlueprintReadOnly, DisplayName="Price(tax incl.)", VisibleDefaultsOnly)
	FString Price;
	
	UPROPERTY(BlueprintReadOnly, DisplayName="Multiple Price", VisibleDefaultsOnly)
	FString MultiplePrice;

	
};

USTRUCT(BlueprintType)
struct FVariationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SCanvas")
	UTexture2D* VariationIcon = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SCanvas")
	FString VariationName;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SCanvas")
	TMap<int32, UMaterialInterface*> MaterialMap;

	UPROPERTY(BlueprintReadWrite, meta=(PinHiddenByDefault), Category="SCanvas")
	int32 VariationIndex = 0;
};


/**
 * 
 */
UCLASS()
class SCANVAS_API USc_PrimaryProductData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	USc_PrimaryProductData();
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FString OriginalMeshName;
	
public:
	
	
	//----------->AssetManagementBegin<-----------//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ProductType")
	FPrimaryAssetType AssetType = "Furniture";
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(AssetType, GetFName());
	}


	UFUNCTION(BlueprintCallable, Category="SCanvas")
	void ForceRefreshData();


#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	//----------->AssetManagementEnd<-----------//
	

	//----------->ProductData<-----------//
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AboutProduct")
	FString BrandName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AboutProduct")
	FString Category;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AboutProduct")
	FString ProductName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Spec")
	UStaticMesh* StaticMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Spec")
	UTexture2D* ProductIcon = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variations")
	TArray<FVariationData> Variations;


	//----------->GUID<-----------//
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, NonTransactional, Category="GUID", meta=(NoResetToDefault, EditCondition="bChangeGuid"))
	FGuid Guid;

	
	//#if WITH_EDITORONLY_DATA
	UFUNCTION(BlueprintCallable, Category="GUID")
	void CheckGuid();
	//#endif

	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="GoogleSheet", meta=(NoResetToDefault))
	UDataTable* AdditionalInfoDataTable = nullptr;

	UFUNCTION(CallInEditor, Category="GoogleSheet")
	void CheckSheet();
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="GoogleSheet")
	FGoogleSheetDataStruct GoogleDataStruct;

	UFUNCTION(BlueprintPure)
	bool CheckAdditionalData(FGoogleSheetDataStruct& Data);

};
