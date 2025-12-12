// Fill out your copyright notice in the Description page of Project Settings.


#include "Sc_ProductActorBase.h"
#include "Sc_PrimaryProductData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASc_ProductActorBase::ASc_ProductActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//Tags.Add(*FGuid::NewGuid().ToString());
	
	
	ProductStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ProductStaticMesh->SetComponentTickEnabled(false);
	ProductStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(ProductStaticMesh);
	//ProductStaticMesh->SetupAttachment(RootScene);
	//SetRootComponent(ProductStaticMesh);
    if (ProductData)
    {
	    UpdateProduct(ProductData);
    }

	ProductStaticMesh->SetCollisionProfileName("ProductPreset");
}

// Called when the game starts or when spawned
void ASc_ProductActorBase::BeginPlay()
{
	Super::BeginPlay();
	Tags.Empty();
	Tags.Add(*FGuid::NewGuid().ToString());
	
}

void ASc_ProductActorBase::UpdateProduct(USc_PrimaryProductData* ProductDataRef)
{
	if (!ProductDataRef){return;}
	
	ProductData = ProductDataRef;
	ProductStaticMesh->SetStaticMesh(ProductData->StaticMesh);
	if (ProductData->Variations.IsValidIndex(0))
	{
		UpdateVariation(ProductData->Variations[0]);	
	}
	//VariationData = ProductData->Variations.IsValidIndex(0) ? ProductData->Variations[0] : FVariationData();
	
	OnProductDataUpdated.Broadcast();
}

void ASc_ProductActorBase::UpdateVariation(FVariationData Variation)
{
	for (const TTuple<int, UMaterialInterface*> MaterialMap : Variation.MaterialMap)
	{
		ProductStaticMesh->SetMaterial(MaterialMap.Key, MaterialMap.Value);
	}
	VariationData = Variation;
}

void ASc_ProductActorBase::UpdateProductFromHistory(USc_PrimaryProductData* ProductDataRef)
{
	if (!ProductDataRef){return;}
	
	ProductData = ProductDataRef;
	ProductStaticMesh->SetStaticMesh(ProductData->StaticMesh);
	OnProductDataUpdated.Broadcast();
	
}

FVector ASc_ProductActorBase::FindEmptyPosition(AActor* ActorToCheck, float Radius)
{
	FVector FoundPosition = ActorToCheck->GetActorLocation();
	
	FVector ActorExtent = ActorToCheck->GetComponentsBoundingBox().GetExtent();

	TArray<FVector> EmptyPositions;

	// Tarama alanını belirleme (örneğin, bir kare alan)
	FVector StartLocation = GetActorLocation();
	//float Radius = 100.0f; // Tarama alanının yarıçapı

	for (float X = -Radius; X <= Radius; X += 10.0f)
	{
		for (float Y = -Radius; Y <= Radius; Y += 10.0f)
		{
			FVector CheckLocation = StartLocation + FVector(X, Y, 10);
			
			// Çakışma kontrolü yapma
			FCollisionQueryParams TraceParams;
			TraceParams.AddIgnoredActor(ActorToCheck); // Kendi objenizi görmezden gelin
			FHitResult HitResult;
			TArray<AActor*> Actors;
			Actors.Add(ActorToCheck);

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(ActorToCheck);
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
			//UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(),CheckLocation, CheckLocation+FVector(0,0,1),ActorExtent, GetActorRotation(),)
			//UKismetSystemLibrary::BoxTraceSingle(GetWorld(),ActorToCheck->GetActorLocation(), CheckLocation+FVector(0,0,1),ActorExtent, GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_WorldStatic),true, Actors, EDrawDebugTrace::None, HitResult,true,FColor::Blue);
			UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(),ActorToCheck->GetActorLocation(), CheckLocation+FVector(0,0,1),ActorExtent, GetActorRotation(),ObjectTypes, true, IgnoreActors,EDrawDebugTrace::ForOneFrame,HitResult,true);
			//bool bHitBox = GetWorld()->LineTraceSingleByChannel(HitResult, CheckLocation, CheckLocation - FVector(0, 0, 1000), ECC_Visibility, TraceParams);
			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CheckLocation, CheckLocation - FVector(0, 0, 1000), ECC_Visibility, TraceParams);

			if (!HitResult.bBlockingHit)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("Blocked")));
				DrawDebugSphere(GetWorld(), HitResult.Location, 25.f,12,FColor::Magenta, false, 2.f);
				// Çakışma yoksa, boş pozisyonu listeye ekleme
				EmptyPositions.Add(HitResult.Location);
				
				//EmptyPositions.Add(CheckLocation);
			}
		}
	}

	// En yakın boş pozisyonu bulma
	FVector NearestEmptyPosition = FVector::ZeroVector;
	float NearestDistance = FLT_MAX;
	for (FVector EmptyPosition : EmptyPositions)
	{
		float Distance = FVector::Distance(StartLocation, EmptyPosition);
		if (Distance < NearestDistance)
		{
			NearestEmptyPosition = EmptyPosition;
			FoundPosition = EmptyPosition;
			NearestDistance = Distance;
		}
	}

	/*
	FVector Origin = ActorToCheck->GetActorLocation();
	
	// Dairenin etrafındaki boş alanı kontrol etmek için bir yörünge oluşturun
	for (float Angle = 0; Angle < 360; Angle += 5) // 5 derece adımlarla dön
	{
		float X = Origin.X + Radius * FMath::Cos(FMath::DegreesToRadians(Angle));
		float Y = Origin.Y + Radius * FMath::Sin(FMath::DegreesToRadians(Angle));
		FVector TestLocation = FVector(X, Y, Origin.Z);

		// TestLocation'da başka bir nesne var mı diye kontrol et
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(ActorToCheck);
		DrawDebugSphere(GetWorld(), TestLocation, 25.f,12,FColor::Magenta, false, 2.f);
		
		auto T = GetWorld()->SweepSingleByObjectType(HitResult,Origin,TestLocation+FVector(0,0,0.1), ActorToCheck->GetActorRotation().Quaternion(), FCollisionObjectQueryParams::AllStaticObjects, FCollisionShape::MakeBox(ActorExtent));
		if (!T)
		{
			FoundPosition = TestLocation;
			break; // Döngüden çık
		}

		// if (!GetWorld()->SweepSingleByChannel(HitResult, TestLocation, TestLocation, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(ActorExtent.Size()), CollisionParams))
		// {
		// 	// Boş alanı bulduysak, SpawnedActor'ı buraya taşı
		// 	//ActorToCheck->SetActorLocation(TestLocation);
		// 	FoundPosition = TestLocation;
		// 	break; // Döngüden çık
		// }
	}*/
	return FoundPosition;
}

bool ASc_ProductActorBase::FindClosestPointOfArray(TArray<FVector> ArrayToSort, FVector& ClosestEmptyPoint) const
{
	if (!ArrayToSort.IsValidIndex(0))
	{
		return false;
	}
	ArrayToSort.Sort([this](const FVector& A, const FVector& B){return UKismetMathLibrary::Vector_Distance(A, GetActorLocation()) < UKismetMathLibrary::Vector_Distance(B, GetActorLocation());});
	ClosestEmptyPoint = ArrayToSort[0];
	
	return true;
}

