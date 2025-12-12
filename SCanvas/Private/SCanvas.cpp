// Copyright Epic Games, Inc. All Rights Reserved.

#include "SCanvas.h"
#define LOCTEXT_NAMESPACE "FSCanvasModule"

void FSCanvasModule::StartupModule()
{

#if WITH_EDITORONLY_DATA
	FString File = FPaths::ProjectConfigDir();
	File.Append(TEXT("DefaultEngine.ini"));
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString FileContent;
	if (FileManager.FileExists(*File))
	{
		if(FFileHelper::LoadFileToString(FileContent,*File,FFileHelper::EHashOptions::None))
		{
			if (FileContent.Contains("GizmoTrace")){return;}
			
			FileContent.Append("\n[/Script/Engine.CollisionProfile]\n+DefaultChannelResponses=(Channel=ECC_GameTraceChannel11,DefaultResponse=ECR_Ignore,bTraceType=True,bStaticObject=False,Name=\"GizmoTrace\")");

			IPlatformFile& WriteFileManager = FPlatformFileManager::Get().GetPlatformFile();
			if (WriteFileManager.FileExists(*File))
			{
				FFileHelper::SaveStringToFile(FileContent,*File);
			}
		}
	}
#endif

	
}

void FSCanvasModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSCanvasModule, SCanvas)