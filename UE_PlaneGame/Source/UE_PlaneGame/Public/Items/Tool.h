// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/WorldItem.h"
#include "Tool.generated.h"

/**
 * Basic tool class for all items that can be held and activated by the player
 */
UCLASS()
class UE_PLANEGAME_API ATool : public AWorldItem
{
	GENERATED_BODY()
    
public:    
	ATool();
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Tool")
	virtual void OnHandMovement(FVector MovementVector);
	
	UFUNCTION(BlueprintCallable, Category = "Tool")
	virtual void OnHandMovementStopped();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Tool")
	void ActivateTool(FVector MovementVector);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Tool")
	void DeactivateTool();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Tool")
	bool bToolActive = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Tool")
	FVector LastMovementDirection;
};