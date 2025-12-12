// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sc_ProductActorBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "TransactionHistorySubsystem.generated.h"


class USc_PrimaryProductData;
class ASc_ProductActorBase;

UENUM(Blueprintable)
enum ETransactActionType : uint8
{
	NonValid UMETA(DisplayName = "None"),
	Movement,
	Switch,
	Variation,
	Deletion,
	Spawning
};

USTRUCT(BlueprintType, Blueprintable)
struct FMovementHistory
{
	GENERATED_BODY()
	FVector PreviousPosition;
	FVector ForwardPosition;
};

USTRUCT(Blueprintable)
struct FTransformStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bUseCustomTransform = false;
	
	UPROPERTY(BlueprintReadWrite)
	FTransform Transform = FTransform();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRefreshProductList);

USTRUCT(BlueprintType, Blueprintable)
struct FTransactAction
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="SCanvas")
	TEnumAsByte<ETransactActionType> ActionType = NonValid;
	
	// UPROPERTY(BlueprintReadWrite)
	// ASc_ProductActorBase* ActorRef = nullptr;
	UPROPERTY()
	USc_PrimaryProductData* CurrentProductData = nullptr;
	UPROPERTY()
	USc_PrimaryProductData* PreviousProductData = nullptr;
	UPROPERTY()
	FVariationData CurrentVariation;
	UPROPERTY()
	FVariationData PreviousVariation;
	UPROPERTY()
	TArray<FName> ActorTag;
	UPROPERTY()
	FTransform StartPosition;
	UPROPERTY()
	FTransform EndPosition;
	UPROPERTY()
	FRotator LastRotation;
	
};



UCLASS()
class SCANVAS_API UTransactionHistorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	UTransactionHistorySubsystem();

	UFUNCTION()
	AActor* FindProductActor(TArray<FName> ActorTag) const;
	
public:
	
	UFUNCTION(BlueprintCallable, Category="SCanvas")
	void AddAction(TEnumAsByte<ETransactActionType> ActionType, ASc_ProductActorBase* ProductRef, FTransformStruct CustomTransform =  FTransformStruct());
	
	UPROPERTY(BlueprintReadOnly, Category="SCanvas")
	int32 ActionIndex = 0;
	
	UPROPERTY(BlueprintReadOnly, Category="SCanvas")
	TArray<FTransactAction> ActionList;

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	bool Back(TEnumAsByte<ETransactActionType> &ActionType);

	UFUNCTION(BlueprintCallable, Category="SCanvas")
	bool Forward(TEnumAsByte<ETransactActionType> &ActionType);

	UPROPERTY(BlueprintReadWrite, Category="SCanvas")
	TSubclassOf<AActor> ProductClassToSpawn;

	UPROPERTY(BlueprintAssignable, Category="SCanvas")
	FRefreshProductList RefreshProductList;
	
};
