// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SC_FunctionLibrary.generated.h"

/**
 * 
 */

class USpotLightComponent;
class UPointLightComponent;

USTRUCT(Blueprintable, BlueprintType)
struct FLightPropertyData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FString PropertyName;

	UPROPERTY(BlueprintReadOnly)
	TArray<uint8> PropertyValue;
	
};


USTRUCT(Blueprintable, BlueprintType)
struct FLightDataV2
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName SocketName = NAME_None;

	UPROPERTY(BlueprintReadOnly)
	TArray<FLightPropertyData> Properties;
	
};



UCLASS()
class SCANVAS_API USC_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SCanvas")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	static bool FindSubfoldersWithObject(UObject* Object, TArray<FString>& SubFolders, int32& FolderCount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SCanvas")
	static bool FindSubFoldersWithPath(const FString& Path, const bool Recursively, TArray<FString>& SubFolders, int32& FolderCount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SCanvas")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	static bool FindStaticMeshesInFolder(const FString& Path, TArray<UMaterialInterface*>& Materials);


	UFUNCTION(BlueprintCallable, Category="SCanvas")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	static bool FillDataTable(UDataTable* DataTable, const FString& String);

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	static void SetLightData(ULightComponent* CopyFrom, ULightComponent* CopyTo);

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	static void ChangeLightRoot(AActor* LightActor, UPointLightComponent* MakeRoot);

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	static void CopyLightSpecs(ULightComponent* CopyFrom, ULightComponent* CopyTo);

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	static TArray<FLightPropertyData> GetLightProperties(ULightComponent* LightComponent);

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	// ReSharper disable once CppUEBlueprintCallableFunctionUnused
	static void WriteLightProperties(ULightComponent* LightComponent, TArray<FLightPropertyData> Properties);

	
#if WITH_EDITORONLY_DATA
	class FNewVisitor final : public IPlatformFile::FDirectoryVisitor
	{
	public:
		TArray<FString> Folders;
		TArray<FString> Files;
		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory)
			{
				//Folders
				Folders.Add(FilenameOrDirectory);
			}
			else
			{
				//Files
				Files.Add(FilenameOrDirectory);
			}
			return true;
		}
	};
#endif



	
	
};


