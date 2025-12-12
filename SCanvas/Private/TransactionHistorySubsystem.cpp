// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppUseStructuredBinding
#include "TransactionHistorySubsystem.h"
#include "Sc_ProductActorBase.h"
#include "Kismet/GameplayStatics.h"


//IMPLEMENT_MODULE(FGoogleApiPluginModule);
UTransactionHistorySubsystem::UTransactionHistorySubsystem()
{
	ActionList.SetNum(50);
}

// ReSharper disable once CppPassValueParameterByConstReference
AActor* UTransactionHistorySubsystem::FindProductActor(const TArray<FName> ActorTag) const
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASc_ProductActorBase::StaticClass(), ActorTag[0],Actors);
	return Actors.IsValidIndex(0) ? Actors[0] : nullptr; 
}

void UTransactionHistorySubsystem::AddAction(const TEnumAsByte<ETransactActionType> ActionType, ASc_ProductActorBase* ProductRef, const FTransformStruct CustomTransform)
{
	if (!ProductRef)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("TransactHistory::AddAction ProductRef is Not Valid")));
		return;
	}
	FTransactAction Action;
	Action.ActionType = ActionType;
	Action.ActorTag = ProductRef->Tags;
	Action.CurrentProductData = ProductRef->ProductData;
	//Action.StartPosition = ProductRef->GetActorLocation();
	Action.StartPosition = CustomTransform.bUseCustomTransform ? CustomTransform.Transform : ProductRef->GetActorTransform();
	Action.CurrentVariation = ProductRef->VariationData;
	Action.LastRotation = ProductRef->GetActorRotation();
	
	if (ActionIndex > 0)
	{
		for (int i = ActionIndex - 1; i >= 0; --i)
		{
			ActionList.RemoveAt(i);
		}
	}
	
	ActionList.Insert(Action, 0);
	ActionList.SetNum(50, true);
	ActionIndex = 0;
}

bool UTransactionHistorySubsystem::Back(TEnumAsByte<ETransactActionType>&ActionType)
{
	if (!ActionList.IsValidIndex(ActionIndex) || ActionList[ActionIndex].ActionType == NonValid)
	{
		ActionType = NonValid;
		return false;
	}
	
	FTransactAction Action = ActionList[ActionIndex];
	ActionType = Action.ActionType;
	switch (Action.ActionType)
	{
		case Movement :
			{
				AActor * TargetActor = FindProductActor(Action.ActorTag);
				if (!TargetActor)
				{
					return false;
				}
				
				//ActionList[ActionIndex].EndPosition = TargetActor->GetActorLocation();
				ActionList[ActionIndex].EndPosition = TargetActor->GetActorTransform();
				//TargetActor->SetActorLocation(Action.StartPosition);
				TargetActor->SetActorTransform(Action.StartPosition);
				ActionIndex++;
				Cast<ASc_ProductActorBase>(TargetActor)->PositionUpdated();
				return true;
			}
		case Switch :
			{
				AActor * TargetActor = FindProductActor(Action.ActorTag);
				ASc_ProductActorBase* Product = Cast<ASc_ProductActorBase>(TargetActor); 
				ActionList[ActionIndex].PreviousProductData = Product->ProductData;
				ActionList[ActionIndex].PreviousVariation = Product->VariationData; 
				Product->UpdateProductFromHistory(Action.CurrentProductData);
				Product->UpdateVariation(Action.CurrentVariation);
				ActionIndex++;
				return true;
			}
		case Variation :
			{
				AActor * TargetActor = FindProductActor(Action.ActorTag);
				ASc_ProductActorBase* Product = Cast<ASc_ProductActorBase>(TargetActor); 
				ActionList[ActionIndex].PreviousVariation = Product->VariationData; 
				Product->UpdateVariation(ActionList[ActionIndex].CurrentVariation);
				ActionIndex++;
				return true;
			}
		case Deletion :
			{
				
				AActor* Actor = GetWorld()->SpawnActor<AActor>(ProductClassToSpawn, Action.StartPosition.GetLocation(), Action.LastRotation, FActorSpawnParameters());
				ASc_ProductActorBase* Product = Cast<ASc_ProductActorBase>(Actor);
				Product->Tags = Action.ActorTag;
				Product->UpdateProduct(Action.CurrentProductData);
				Product->UpdateVariation(Action.CurrentVariation);
				ActionIndex++;
				RefreshProductList.Broadcast();
				return true;
			}
		case Spawning :
			{
				AActor * TargetActor = FindProductActor(Action.ActorTag);
				Action.LastRotation = TargetActor->GetActorRotation();
				TargetActor->Destroy();
				//AddAction(Deletion, Cast<ASc_ProductActorBase>(TargetActor));
				ActionIndex++;
				RefreshProductList.Broadcast();
				return true;
			}
		default: {}
	}
	
	ActionIndex++;
	return true;
}

bool UTransactionHistorySubsystem::Forward(TEnumAsByte<ETransactActionType>& ActionType)
{
	if (!ActionList.IsValidIndex(ActionIndex-1) || ActionList[ActionIndex-1].ActionType == NonValid)
	{
		ActionType = NonValid;
		return false;
	}
	
	ActionIndex--;
	const FTransactAction Action = ActionList[ActionIndex];
	ActionType = Action.ActionType;
	switch (Action.ActionType)
	{
	case Movement :
		{
			AActor * TargetActor = FindProductActor(Action.ActorTag);
			if (!TargetActor)
			{
				return false;
			}
			
			//TargetActor->SetActorLocation(Action.EndPosition);
			TargetActor->SetActorTransform(Action.EndPosition);
			Cast<ASc_ProductActorBase>(TargetActor)->PositionUpdated();
			return true;
		}
	case Switch :
		{
			AActor * TargetActor = FindProductActor(Action.ActorTag);
			ASc_ProductActorBase* Product = Cast<ASc_ProductActorBase>(TargetActor); 
			Product->UpdateProductFromHistory(Action.PreviousProductData);
			Product->UpdateVariation(Action.PreviousVariation);
			return true;
		}
	case Variation :
		{
			AActor * TargetActor = FindProductActor(Action.ActorTag);
			ASc_ProductActorBase* Product = Cast<ASc_ProductActorBase>(TargetActor); 
			Product->UpdateVariation(Action.PreviousVariation);
			return true;
		}
	case Deletion :
		{
			AActor * TargetActor = FindProductActor(Action.ActorTag);
			TargetActor->Destroy();
			RefreshProductList.Broadcast();
			return true;
		}
	case Spawning :
		{
			AActor* Actor = GetWorld()->SpawnActor<AActor>(ProductClassToSpawn, Action.StartPosition.GetLocation(), Action.LastRotation, FActorSpawnParameters());
			ASc_ProductActorBase* Product = Cast<ASc_ProductActorBase>(Actor);
			Product->Tags = Action.ActorTag;
			Product->UpdateProduct(Action.CurrentProductData);
			Product->UpdateVariation(Action.CurrentVariation);
			RefreshProductList.Broadcast();
			return true;
		}
	default: {}
	}

	
	return true;
}
