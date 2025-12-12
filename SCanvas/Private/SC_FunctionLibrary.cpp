// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_FunctionLibrary.h"

#include "Components/PointLightComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

bool USC_FunctionLibrary::FindSubfoldersWithObject(UObject* Object, TArray<FString>& SubFolders, int32& FolderCount)
{
#if WITH_EDITORONLY_DATA
	if (Object)
	{
		const FString Path = FPaths::GetPath(UKismetSystemLibrary::GetSystemPath(Object));
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		FNewVisitor Visitor;
		//PlatformFile.IterateDirectory(*Path, Visitor);
		PlatformFile.IterateDirectoryRecursively(*Path, Visitor);
		SubFolders = Visitor.Folders;
		FolderCount = Visitor.Folders.Num();
		
		// TArray<FString> Directories;
		// FFileManagerGeneric::Get().FindFilesRecursive(Directories, *Path, TEXT("*"), false, true, true);
		// SubFolders = Directories;
		// FolderCount = Directories.Num();
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Path);
		return true;
	}
#endif
	return false;
}

bool USC_FunctionLibrary::FindSubFoldersWithPath(const FString& Path, const bool Recursively, TArray<FString>& SubFolders, int32& FolderCount)
{
#if WITH_EDITORONLY_DATA
	if (!FPaths::DirectoryExists(Path))
	{
		return false;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FNewVisitor Visitor;
	if (Recursively)
	{
		PlatformFile.IterateDirectoryRecursively(*Path, Visitor);	
	}
	else
	{
		PlatformFile.IterateDirectory(*Path, Visitor);	
	}
	
	SubFolders = Visitor.Folders;
	FolderCount = Visitor.Folders.Num();
#endif
	
	return true;
}



bool USC_FunctionLibrary::FindStaticMeshesInFolder(const FString& Path, TArray<UMaterialInterface*>& Materials)
{
#if WITH_EDITORONLY_DATA
	if (!FPaths::DirectoryExists(Path))
	{
		return false;
	}
#endif
	return true;
	
}

bool USC_FunctionLibrary::FillDataTable(UDataTable* DataTable, const FString& String)
{
#if WITH_EDITORONLY_DATA
	if (!DataTable)
	{
		return false;
	}
	return UDataTableFunctionLibrary::FillDataTableFromCSVString(DataTable, String);
#endif
	return true;
}

void USC_FunctionLibrary::SetLightData(ULightComponent* CopyFrom, ULightComponent* CopyTo)
{
	const FTransform Transform = CopyFrom->GetRelativeTransform();
	//UPointLightComponent* NewLight = NewObject<UPointLightComponent>(CopyFrom->GetOuter(), CopyFrom->GetClass(), NAME_None, RF_NoFlags);
	//NewLight->SetupAttachment(GetRootComponent());
	CopyFrom = CopyTo;

	if (const auto PointLight = Cast<UPointLightComponent>(CopyFrom))
	{
		auto T = Cast<UPointLightComponent>(CopyTo);
		T = PointLight;
	}
	
	const auto NewLight = DuplicateObject<ULightComponent>(CopyFrom, CopyTo->GetOuter());
	//NewLight->SetupAttachment(this->GetRootComponent());
	CopyTo->GetOwner()->SetRootComponent(NewLight);
	//NewLight->AttachToComponent(CopyTo->GetOwner()->GetRootComponent(),FAttachmentTransformRules::SnapToTargetIncludingScale);
	NewLight->SetRelativeTransform(Transform);
	NewLight->RegisterComponent();

	CopyTo->DestroyComponent();
	CopyFrom->GetOwner()->Destroy();
}

void USC_FunctionLibrary::ChangeLightRoot(AActor* LightActor, UPointLightComponent* MakeRoot)
{
	//MakeRoot->SetupAttachment(LightActor->GetRootComponent());
	LightActor->SetRootComponent(MakeRoot);
}

void USC_FunctionLibrary::CopyLightSpecs(ULightComponent* CopyFrom, ULightComponent* CopyTo)
{

	if (!CopyFrom || !CopyTo)
	{
		UE_LOG(LogTemp, Warning, TEXT("CopyLightSpecs: Invalid input ptr"));
		return;
	}

	//Check Classes are same
	if (CopyFrom->GetClass() != CopyTo->GetClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("CopyLightSpecs: Ligth classes are not match"));
		return;
	}

	const UClass* LightClass = CopyFrom->GetClass();
	for (TFieldIterator<FProperty> PropIt(LightClass); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;

		//Check only editable properties
		if (!Property->HasAnyPropertyFlags(CPF_Edit | CPF_BlueprintVisible))
			continue;

		//Copy Spec
		const void* FromValuePtr = Property->ContainerPtrToValuePtr<void>(CopyFrom);
		void* ToValuePtr = Property->ContainerPtrToValuePtr<void>(CopyTo);
		Property->CopyCompleteValue(ToValuePtr, FromValuePtr);
	}
	
	//Mark RenderState is dirty to apply new updates
	CopyTo->MarkRenderStateDirty();
	
}

TArray<FLightPropertyData> USC_FunctionLibrary::GetLightProperties(ULightComponent* LightComponent)
{
	TArray<FLightPropertyData> SavedProperties;

	const UClass* LightClass = LightComponent->GetClass();
	
	
	for (TFieldIterator<FProperty> PropIt(LightClass); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;

		// Sadece düzenlenebilir özellikleri kontrol et
		if (!Property->HasAnyPropertyFlags(CPF_Edit | CPF_BlueprintVisible))
			continue;
		
		FLightPropertyData PropertyData;
		PropertyData.PropertyName = Property->GetName();

		// Özellik değerini bir byte dizisine serialize et
		PropertyData.PropertyValue.SetNum(Property->GetSize());
		Property->CopySingleValue(PropertyData.PropertyValue.GetData(), Property->ContainerPtrToValuePtr<void>(LightComponent));

		SavedProperties.Add(PropertyData);
	}
	
	return SavedProperties;
}

void USC_FunctionLibrary::WriteLightProperties(ULightComponent* LightComponent,TArray<FLightPropertyData> Properties)
{
	//ReSharper disable once CppUseStructuredBinding
	for (const FLightPropertyData& PropertyData : Properties)
	{
		//ReSharper disable once CppTooWideScope
		const FProperty* Property = LightComponent->GetClass()->FindPropertyByName(*PropertyData.PropertyName);
		if (Property)
		{
			Property->CopySingleValue(Property->ContainerPtrToValuePtr<void>(LightComponent), PropertyData.PropertyValue.GetData());
		}
	}
	LightComponent->MarkRenderStateDirty();
	
}
