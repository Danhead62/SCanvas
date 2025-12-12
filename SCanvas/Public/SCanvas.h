// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSCanvasModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

UENUM(Blueprintable)
enum EGizmoAxis
{
	None,
	X,Y,Z
};

UENUM(Blueprintable)
enum EProductDragMode
{
	Horizontal,
	Vertical
};
