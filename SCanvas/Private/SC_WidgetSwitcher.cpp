// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_WidgetSwitcher.h"

void USC_WidgetSwitcher::SetActiveWidgetIndex(int32 Index)
{
	Super::SetActiveWidgetIndex(Index);
	OnWidgetSwitch.Broadcast(Index);
}
