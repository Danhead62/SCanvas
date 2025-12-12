// Fill out your copyright notice in the Description page of Project Settings.


#include "Sc_PrimaryProductData.h"
#include "Engine/AssetManager.h"
#include "Kismet/DataTableFunctionLibrary.h"


USc_PrimaryProductData::USc_PrimaryProductData()
{
// #if WITH_EDITORONLY_DATA
// 	if (StaticMesh)
// 	{
// 		UEditorAssetSubsystem* EditorAssetSubsystem = GEditor ?  GEditor->GetEditorSubsystem<UEditorAssetSubsystem>() : nullptr;
// 		OriginalMeshName = EditorAssetSubsystem->GetMetadataTag(StaticMesh, TEXT("product_name"));
// 		UE_LOG(LogTemp, Warning, TEXT("EditorSubsystem is Valid"));
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("EditorSubsystem is not Valid"));
// 	}
// #endif

	const auto FindDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(TEXT("/Script/Engine.DataTable'/SCanvas/Tools/GoogleSheets/DT_ProductAdditionalInfoTable.DT_ProductAdditionalInfoTable'"));
	if (FindDataTable.Succeeded())
	{
		AdditionalInfoDataTable = FindDataTable.Object;
	}
	
	

	
}

#if WITH_EDITORONLY_DATA

void USc_PrimaryProductData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//if asset type edited, this code will refresh data asset on memory.
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USc_PrimaryProductData, AssetType))
	{
		UAssetManager::Get().RefreshAssetData(this);
		//UAssetManager::Get().RefreshPrimaryAssetDirectory(true);
	}

	//if asset type edited, this code will refresh data asset on memory.
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USc_PrimaryProductData, StaticMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("Static Mesh Updated"));
		//UAssetManager::Get().RefreshPrimaryAssetDirectory(true);
	}
}
#endif

//#if WITH_EDITORONLY_DATA
void USc_PrimaryProductData::CheckGuid()
{
	UE_LOG(LogTemp, Error, TEXT("Guid Check"));
	if (!Guid.IsValid())
	{
		//UE_LOG(LogTemp, Error, TEXT("Guid Update"));
		Guid = FGuid::NewGuid();
	}	
}

//#endif


void USc_PrimaryProductData::ForceRefreshData()
{
#if WITH_EDITORONLY_DATA
	UAssetManager::Get().RefreshAssetData(this);
	UAssetManager::Get().RefreshPrimaryAssetDirectory(true);
#endif
}

void USc_PrimaryProductData::CheckSheet()
{
	FGoogleSheetDataStruct Data;
	CheckAdditionalData(Data);
}

bool USc_PrimaryProductData::CheckAdditionalData(FGoogleSheetDataStruct& Data)
{
	if (!AdditionalInfoDataTable)
	{
		return false;
	}
	
	const TArray<FString> SearchProductNameEng = UDataTableFunctionLibrary::GetDataTableColumnAsString(AdditionalInfoDataTable, "ProductNameEnglish");
	const int32 FoundIndex = SearchProductNameEng.Find(ProductName)+1;
	ProductName.TrimEndInline();
	//auto Index = (TEXT("%i"),FoundIndex);
	UE_LOG(LogTemp, Warning, TEXT("Row Not Found %s"), *FString::FromInt(FoundIndex));
	if (FoundIndex != INDEX_NONE && FoundIndex-1 > -1)
	{
		if (!UDataTableFunctionLibrary::DoesDataTableRowExist(AdditionalInfoDataTable, *FString::FromInt(FoundIndex)))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Row Doesn't Exist"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("USc_PrimaryProductData::CheckAdditionalData->Index Doesn't exist %s: "), *ProductName));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("USc_PrimaryProductData::CheckAdditionalData->Index Doesn't exist %i: "), FoundIndex));
			return false;
		}
		const FGoogleSheetDataStruct* T  = AdditionalInfoDataTable->FindRow<FGoogleSheetDataStruct>(*FString::FromInt(FoundIndex),nullptr);
		GoogleDataStruct = *T;
		T = nullptr;
		//AdditionalInfoDataTable = nullptr;
		Data = GoogleDataStruct;
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Row Doesn't exist %s:"), *ProductName));
		
		// auto T = (TEXT("%i"),FoundIndex);
		// UE_LOG(LogTemp, Warning, TEXT("Row Not Found %s"), *FString::FromInt(FoundIndex));
	}
	
	return false;

}

