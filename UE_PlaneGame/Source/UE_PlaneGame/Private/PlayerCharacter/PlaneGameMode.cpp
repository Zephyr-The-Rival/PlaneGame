// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_PlaneGame/Public/PlayerCharacter/PlaneGameMode.h"

#include "UE_PlaneGame/Public/PlayerCharacter/PlanePlayerController.h"
#include "UE_PlaneGame/Public/PlayerCharacter/PlanePlayerCharacter.h"

APlaneGameMode::APlaneGameMode()
{
	DefaultPawnClass = APlanePlayerCharacter::StaticClass();
	PlayerControllerClass = APlanePlayerController::StaticClass();
}
