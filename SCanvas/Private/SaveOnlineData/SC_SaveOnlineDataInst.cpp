// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveOnlineData/SC_SaveOnlineDataInst.h"

#include "JsonObjectConverter.h"
#include "SC_FunctionLibrary.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/PointLight.h"
#include "Engine/SpotLight.h"
#include "Kismet/GameplayStatics.h"


FString USC_SaveOnlineDataInst::GetLevelDataAsJson()
{
	if (!GetWorld())
	{
		return FString();
	}

	TSharedPtr<FJsonObject> RootJson = MakeShared<FJsonObject>();
	
	OnlineSaveDataString = TArray<FOnlineSaveDataString>();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASc_ProductActorBase::StaticClass(), FoundActors);

	TArray<TSharedPtr<FJsonValue>> ProductsArrayJson;
	for (AActor* FoundActor : FoundActors)
	{
		TSharedPtr<FJsonObject> ProductJson = MakeShared<FJsonObject>();
		FOnlineSaveDataString NewSaveData;
		const auto Product = Cast<ASc_ProductActorBase>(FoundActor);
		ProductJson->SetStringField("ActorTransform", Product->GetActorTransform().ToString());
		FSoftObjectPath AssetPath = Product->ProductData->GetPathName();
		ProductJson->SetStringField("ProductPath", AssetPath.ToString());
		ProductJson->SetNumberField("SelectedVariation", Product->VariationData.VariationIndex);
		
		TSharedPtr<FJsonObject> LightJsonData = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> SpotLightJson;
		TArray<TSharedPtr<FJsonValue>> PointLightJson;
		TArray<TSharedPtr<FJsonValue>> LightProperties;
		TArray<FLightData> NewLightDataList;
		TArray<AActor*> ChildActors;
		
		for (UActorComponent* BlueprintCreatedComponent : Product->BlueprintCreatedComponents)
		{
			auto ChildActorComponent = Cast<UChildActorComponent>(BlueprintCreatedComponent);
			if (!ChildActorComponent){continue;}
			auto ChildActor = ChildActorComponent->GetChildActor();
			
			//TODO:
			if (ChildActor->IsA(ALight::StaticClass()))
			{
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
				
				ULightComponent* LightComponent = Cast<ALight>(ChildActor)->GetLightComponent();
				JsonObject->SetStringField("SocketName", ChildActorComponent->GetAttachSocketName().ToString());
				
				TArray<FLightPropertyData> Properties = USC_FunctionLibrary::GetLightProperties(LightComponent);
				
				TArray<TSharedPtr<FJsonValue>> PropertiesJsonArray;

				for (const FLightPropertyData& Data : Properties)
				{
					TSharedPtr<FJsonObject> PropertyJsonObject = MakeShared<FJsonObject>();
					PropertyJsonObject->SetStringField("PropertyName", Data.PropertyName);
					PropertyJsonObject->SetStringField("PropertyValue", FBase64::Encode(Data.PropertyValue));
					PropertiesJsonArray.Add(MakeShared<FJsonValueObject>(PropertyJsonObject));
					
				}
				JsonObject->SetArrayField("LightProperties", PropertiesJsonArray);
				TSharedPtr<FJsonValueObject> JsonValueObject = MakeShared<FJsonValueObject>(JsonObject);
				LightProperties.Add(JsonValueObject);
			}
			
			FLightData LightData;
			if (ChildActor->IsA(APointLight::StaticClass()))
			{
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
				
				UPointLightComponent* PointLightComponent = Cast<APointLight>(ChildActor)->PointLightComponent;
				JsonObject->SetStringField("SocketName", ChildActorComponent->GetAttachSocketName().ToString());
				JsonObject->SetNumberField("LightIntensity", PointLightComponent->Intensity);
				JsonObject->SetNumberField("LightTemperature", PointLightComponent->Temperature);
				JsonObject->SetNumberField("AttentionRadius", PointLightComponent->AttenuationRadius);
				JsonObject->SetBoolField("bCastShadow", PointLightComponent->CastShadows);
				JsonObject->SetNumberField("SourceRadius", PointLightComponent->SourceRadius);
				JsonObject->SetNumberField("IndirectLightingIntensity", PointLightComponent->IndirectLightingIntensity);
				JsonObject->SetNumberField("SoftSourceRadius", PointLightComponent->SoftSourceRadius);
				JsonObject->SetNumberField("SourceLength", PointLightComponent->SourceLength);

				//RayTracedShadow
				JsonObject->SetBoolField("bRayTracedShadow", PointLightComponent->CastRaytracedShadow != ECastRayTracedShadow::Disabled);
				
				TSharedPtr<FJsonValueObject> JsonValueObject = MakeShared<FJsonValueObject>(JsonObject);
				PointLightJson.Add(JsonValueObject);
			}
			else if (ChildActor->IsA(ASpotLight::StaticClass()))
			{
				USpotLightComponent* SpotLightComponent = Cast<ASpotLight>(ChildActor)->SpotLightComponent;
				LightData.SocketName = ChildActorComponent->GetAttachSocketName();
				LightData.LightIntensity = SpotLightComponent->Intensity;
				LightData.LightTemperature = SpotLightComponent->Temperature;
				LightData.AttentionRadius = SpotLightComponent->AttenuationRadius;
				LightData.bCastShadow = SpotLightComponent->CastShadows;
				LightData.SourceRadius = SpotLightComponent->SourceRadius;
				LightData.IndirectLightingIntensity = SpotLightComponent->IndirectLightingIntensity;
				LightData.SoftSourceRadius = SpotLightComponent->SoftSourceRadius;
				LightData.InnerCone = SpotLightComponent->InnerConeAngle;
				LightData.OuterCone = SpotLightComponent->OuterConeAngle;
				LightData.SourceLength = SpotLightComponent->SourceLength;
				//RayTracedShadow
				LightData.bRayTracedShadow = SpotLightComponent->CastRaytracedShadow != ECastRayTracedShadow::Disabled;
				
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
				JsonObject->SetStringField("SocketName", ChildActorComponent->GetAttachSocketName().ToString());
				JsonObject->SetNumberField("LightIntensity", SpotLightComponent->Intensity);
				JsonObject->SetNumberField("LightTemperature", SpotLightComponent->Temperature);
				JsonObject->SetNumberField("AttentionRadius", SpotLightComponent->AttenuationRadius);
				JsonObject->SetBoolField("bCastShadow", SpotLightComponent->CastShadows);
				JsonObject->SetNumberField("SourceRadius", SpotLightComponent->SourceRadius);
				JsonObject->SetNumberField("IndirectLightingIntensity", SpotLightComponent->IndirectLightingIntensity);
				JsonObject->SetNumberField("SoftSourceRadius", SpotLightComponent->SoftSourceRadius);
				JsonObject->SetNumberField("SourceLength", SpotLightComponent->SourceLength);
				JsonObject->SetNumberField("InnerCone", SpotLightComponent->InnerConeAngle);
				JsonObject->SetNumberField("OuterCone", SpotLightComponent->OuterConeAngle);
				//RayTracedShadow
				JsonObject->SetBoolField("bRayTracedShadows",SpotLightComponent->CastRaytracedShadow != ECastRayTracedShadow::Disabled);
				
				TSharedPtr<FJsonValueObject> JsonValueObject = MakeShared<FJsonValueObject>(JsonObject);
				SpotLightJson.Add(JsonValueObject);
			}
			NewLightDataList.Add(LightData);
		}
		LightJsonData->SetArrayField("PointLights",PointLightJson);
		LightJsonData->SetArrayField("SpotLights",SpotLightJson);
		LightJsonData->SetArrayField("LightProperties",LightProperties);
		
		ProductJson->SetObjectField("LightData", LightJsonData);
		TSharedPtr<FJsonValueObject> ProductJsonValue = MakeShared<FJsonValueObject>(ProductJson);
		ProductsArrayJson.Add(ProductJsonValue);
		//Product->Destroy();
	}
	TSharedPtr<FJsonObject> Level = MakeShared<FJsonObject>();
	RootJson->SetStringField("LevelName", UGameplayStatics::GetCurrentLevelName(GetWorld()));
	RootJson->SetArrayField("Products", ProductsArrayJson);
	
	FString JsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(RootJson.ToSharedRef(), JsonWriter);

	//This is save json as a text file
	//FString FilePath = FPaths::ProjectDir() + "Saved/JsonData.txt";
	//FFileHelper::SaveStringToFile(JsonString, *FilePath);
	return JsonString;
}

bool USC_SaveOnlineDataInst::SCGetFieldValue(FString& Value, const FString& JsonString, const FString& Field)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return false;
	}
	if (!JsonObject->HasField(Field))
	{
		return false;
	}
	Value = JsonObject->GetStringField(Field);
	return true;
}

bool USC_SaveOnlineDataInst::LoadLevelFromJsonData(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return false;
	}
	if (!JsonObject->HasField("Products")) return false;
	
	FName BP_ProductActorClass = TEXT("Class'/SCanvas/Blueprints/BP_ProductActor.BP_ProductActor_C'");
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *BP_ProductActorClass.ToString()));
	
	for (const auto ProductJson : JsonObject->GetArrayField("Products"))
	{
		const auto ProductJsonObject = ProductJson->AsObject();

		//Get Transform
		FString ActorTransformString;
		if (!ProductJsonObject->TryGetStringField("ActorTransform", ActorTransformString)) continue;
		FTransform ActorTransform;
		if (!ActorTransform.InitFromString(ActorTransformString)) continue;

		//ProductDataAsset
		FString ProductDataPath;
		if (!ProductJsonObject->TryGetStringField("ProductPath", ProductDataPath)) continue;
		TSoftObjectPtr<USc_PrimaryProductData> ProductDataSoftPath;
		ProductDataSoftPath = ProductDataPath;
		USc_PrimaryProductData* ProductDataAsset = Cast<USc_PrimaryProductData>(UKismetSystemLibrary::LoadAsset_Blocking(ProductDataSoftPath)); 
		if (!ProductDataAsset) continue;
		
		//VariationIndex
		float VariationIndex;
		if (!ProductJsonObject->TryGetNumberField("SelectedVariation",VariationIndex)) continue;

		//LightData
		TArray<FLightData> LightData;
		LightData.Empty();

		TArray<FLightDataV2> LightPropertyData;
		LightPropertyData.Empty();
		
		TSharedPtr<FJsonObject> LightDataObject = ProductJsonObject->GetObjectField("LightData");
		if (LightDataObject.IsValid())
		{
			TArray<TSharedPtr<FJsonValue>> LightPropertiesArray = LightDataObject->GetArrayField("LightProperties");
			
			for (TSharedPtr<FJsonValue> LightProperty : LightPropertiesArray)
			{
				FLightDataV2 LightDataV2S;
				LightDataV2S.SocketName = *LightProperty->AsObject()->GetStringField("SocketName");

				// TArray<FString> Fields;
				// LightProperty->AsObject().Get()->Values.GetKeys(Fields);
				// for (FString Field : Fields)
				// {
				// 	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Orange, FString::Printf(TEXT(" %s"), *Field));
				// } 
				
				auto Properties = LightProperty.Get()->AsObject()->GetArrayField("LightProperties");
				for (TSharedPtr<FJsonValue> Property : Properties)
				{
					//

					FLightPropertyData PropertyList;
					PropertyList.PropertyName = Property.Get()->AsObject()->GetStringField("PropertyName");
					FString Base64Value = Property.Get()->AsObject()->GetStringField("PropertyValue");
					FBase64::Decode(Base64Value, PropertyList.PropertyValue);

					LightDataV2S.Properties.Add(PropertyList);
					
					
				} 
				LightPropertyData.Add(LightDataV2S);
				//
				// // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("Işık")));
				// // JSON verisini FString'e çevir
				// FString OutputString;
				// TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
				// FJsonSerializer::Serialize(LightProperty->AsObject().ToSharedRef(), Writer);
				//
				// // Dosya yolu belirle
				// FString FilePath = FPaths::ProjectDir() + TEXT("Test.json");
				// FFileHelper::SaveStringToFile(OutputString, *FilePath);

			}
			
			
			
			TArray<TSharedPtr<FJsonValue>> PointLightsArray = LightDataObject->GetArrayField("PointLights");
			for (const auto& PointLightValue : PointLightsArray)
			{
				
				FLightData PointLightData;
				TSharedPtr<FJsonObject> PointLightObject  = PointLightValue->AsObject();
				
				PointLightData.SocketName = *PointLightObject->GetStringField("SocketName");
				PointLightData.LightIntensity = PointLightObject->GetNumberField("LightIntensity");
				PointLightData.LightTemperature = PointLightObject->GetNumberField("LightTemperature");
				PointLightData.AttentionRadius = PointLightObject->GetNumberField("AttentionRadius");	
				PointLightData.bCastShadow = PointLightObject->GetBoolField("bCastShadow");
				PointLightData.SourceRadius = PointLightObject->GetNumberField("SourceRadius");
				PointLightData.IndirectLightingIntensity = PointLightObject->GetNumberField("IndirectLightingIntensity");
				PointLightData.SoftSourceRadius = PointLightObject->GetNumberField("SoftSourceRadius");
				PointLightData.SourceLength = PointLightObject->GetNumberField("SourceLength");
				//RayTracedShadow
				PointLightData.bRayTracedShadow = PointLightObject->GetBoolField("bRayTracedShadows");
				LightData.Add(PointLightData);
			}
			TArray<TSharedPtr<FJsonValue>> SpotLightsArray = LightDataObject->GetArrayField("SpotLights");
			for (const auto& SpotLightValue : SpotLightsArray)
			{
				FLightData SpotLightData;
				TSharedPtr<FJsonObject> PointLightObject  = SpotLightValue->AsObject();
				
				SpotLightData.SocketName = *PointLightObject->GetStringField("SocketName");
				SpotLightData.LightIntensity = PointLightObject->GetNumberField("LightIntensity");
				SpotLightData.LightTemperature = PointLightObject->GetNumberField("LightTemperature");
				SpotLightData.AttentionRadius = PointLightObject->GetNumberField("AttentionRadius");	
				SpotLightData.bCastShadow = PointLightObject->GetBoolField("bCastShadow");
				SpotLightData.SourceRadius = PointLightObject->GetNumberField("SourceRadius");
				SpotLightData.IndirectLightingIntensity = PointLightObject->GetNumberField("IndirectLightingIntensity");
				SpotLightData.SoftSourceRadius = PointLightObject->GetNumberField("SoftSourceRadius");
				SpotLightData.SourceLength = PointLightObject->GetNumberField("SourceLength");
				SpotLightData.InnerCone = PointLightObject->GetNumberField("InnerCone");
				SpotLightData.OuterCone = PointLightObject->GetNumberField("OuterCone");
				//RayTracedShadow
				SpotLightData.bRayTracedShadow = PointLightObject->GetBoolField("bRayTracedShadows");
				LightData.Add(SpotLightData);
			}
		}
		
		//SpawnActor
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//ASc_ProductActorBase* NewActor = GetWorld()->SpawnActorDeferred<ASc_ProductActorBase>(ASc_ProductActorBase::StaticClass(), SaveData.ActorTransform);
		AActor *NewActor = GetWorld()->SpawnActorDeferred<AActor>(GeneratedBP, ActorTransform);
		if (!NewActor) continue;
		ASc_ProductActorBase* NewProduct = Cast<ASc_ProductActorBase>(NewActor);

		NewProduct->bCheckEmptyPosition = false;
		NewProduct->ProductData = ProductDataAsset;
		NewProduct->VariationData = ProductDataAsset->Variations[VariationIndex];
		NewProduct->LightDataList = LightData;
		NewProduct->LightPropertyList = LightPropertyData;
		
		NewProduct->FinishSpawning(ActorTransform);
		NewProduct->UpdateProduct(NewProduct->ProductData);
		NewProduct->UpdateVariation(ProductDataAsset->Variations[VariationIndex]);
		NewProduct->VariationData.VariationIndex = VariationIndex;

	} 
	
	return true;
}


FString USC_SaveOnlineDataInst::GetLevelDataAsJsonV2()
{
		if (!GetWorld())
	{
		return FString();
	}

	TSharedPtr<FJsonObject> RootJson = MakeShared<FJsonObject>();
	
	OnlineSaveDataString = TArray<FOnlineSaveDataString>();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASc_ProductActorBase::StaticClass(), FoundActors);

	TArray<TSharedPtr<FJsonValue>> ProductsArrayJson;
	for (AActor* FoundActor : FoundActors)
	{
		TSharedPtr<FJsonObject> ProductJson = MakeShared<FJsonObject>();
		FOnlineSaveDataString NewSaveData;
		const auto Product = Cast<ASc_ProductActorBase>(FoundActor);
		ProductJson->SetStringField("ActorTransform", Product->GetActorTransform().ToString());
		FSoftObjectPath AssetPath = Product->ProductData->GetPathName();
		ProductJson->SetStringField("ProductPath", AssetPath.ToString());
		ProductJson->SetNumberField("SelectedVariation", Product->VariationData.VariationIndex);
		
		TSharedPtr<FJsonObject> LightJsonData = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> LightProperties;
		TArray<FLightData> NewLightDataList;
		TArray<AActor*> ChildActors;
		
		for (UActorComponent* BlueprintCreatedComponent : Product->BlueprintCreatedComponents)
		{
			auto ChildActorComponent = Cast<UChildActorComponent>(BlueprintCreatedComponent);
			if (!ChildActorComponent){continue;}
			auto ChildActor = ChildActorComponent->GetChildActor();
			
			if (ChildActor->IsA(ALight::StaticClass()))
			{
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
				
				ULightComponent* LightComponent = Cast<ALight>(ChildActor)->GetLightComponent();
				JsonObject->SetStringField("SocketName", ChildActorComponent->GetAttachSocketName().ToString());
				
				TArray<FLightPropertyData> Properties = USC_FunctionLibrary::GetLightProperties(LightComponent);
				
				TArray<TSharedPtr<FJsonValue>> PropertiesJsonArray;

				for (const FLightPropertyData& Data : Properties)
				{
					TSharedPtr<FJsonObject> PropertyJsonObject = MakeShared<FJsonObject>();
					PropertyJsonObject->SetStringField("PropertyName", Data.PropertyName);
					PropertyJsonObject->SetStringField("PropertyValue", FBase64::Encode(Data.PropertyValue));
					PropertiesJsonArray.Add(MakeShared<FJsonValueObject>(PropertyJsonObject));
					
				}
				
				JsonObject->SetArrayField("LightProperties", PropertiesJsonArray);
				TSharedPtr<FJsonValueObject> JsonValueObject = MakeShared<FJsonValueObject>(JsonObject);
				LightProperties.Add(JsonValueObject);

				
				// // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("Işık")));
				// // JSON verisini FString'e çevir
				// FString OutputString;
				// TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
				// FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
				//
				// // Dosya yolu belirle
				// FString FilePath = FPaths::ProjectDir() + TEXT("Test.json");
				// FFileHelper::SaveStringToFile(OutputString, *FilePath);

			}
		}
		
		LightJsonData->SetArrayField("LightProperties",LightProperties);
		
		ProductJson->SetObjectField("LightData", LightJsonData);
		TSharedPtr<FJsonValueObject> ProductJsonValue = MakeShared<FJsonValueObject>(ProductJson);
		ProductsArrayJson.Add(ProductJsonValue);
	}
	TSharedPtr<FJsonObject> Level = MakeShared<FJsonObject>();
	RootJson->SetStringField("LevelName", UGameplayStatics::GetCurrentLevelName(GetWorld()));
	RootJson->SetArrayField("Products", ProductsArrayJson);
	
	FString JsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(RootJson.ToSharedRef(), JsonWriter);

	//This is save json as a text file
	//FString FilePath = FPaths::ProjectDir() + "Saved/JsonData.txt";
	//FFileHelper::SaveStringToFile(JsonString, *FilePath);
	return JsonString;
}
















