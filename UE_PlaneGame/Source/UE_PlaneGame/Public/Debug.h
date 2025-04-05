// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UE_PLANEGAME_API Debug
{
public:
	static void Print(FString Text, float TimeToDisplay =5, FColor Color = FColor::Cyan);
};
