// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_CustomPointLight.h"

#include "Components/PointLightComponent.h"

void ASC_CustomPointLight::UpdatePointLight(UPointLightComponent* CopyFrom, UPointLightComponent* CopyTo)
{

	FTransform Transform = CopyFrom->GetRelativeTransform();
	//UPointLightComponent* NewLight = NewObject<UPointLightComponent>(CopyFrom->GetOuter(), CopyFrom->GetClass(), NAME_None, RF_NoFlags);
	//NewLight->SetupAttachment(GetRootComponent());

	const auto NewLight = DuplicateObject<UPointLightComponent>(CopyFrom, this);
	
	//NewLight->SetupAttachment(this->GetRootComponent());
	NewLight->AttachToComponent(this->GetRootComponent(),FAttachmentTransformRules::SnapToTargetIncludingScale);
	NewLight->RegisterComponent();
	NewLight->SetRelativeTransform(Transform);
	CopyTo->DestroyComponent();
	CopyFrom->GetOwner()->Destroy();
}
