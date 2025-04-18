// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/WorldItem.h"
#include "Tool.generated.h"

UCLASS()
class UE_PLANEGAME_API ATool : public AWorldItem
{
	GENERATED_BODY()
    
public:    
	ATool();

	virtual void Tick(float DeltaTime) override;
    
	void OnHandMovement(FVector MovementVector);
	void OnHandMovementStopped();
    
	UFUNCTION(BlueprintNativeEvent)
	void ActivateTool(FVector MovementVector);
	void ActivateTool_Implementation(FVector MovementVector);
    
	UFUNCTION(BlueprintNativeEvent)
	void DeactivateTool();
	void DeactivateTool_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnMovementThresholdReached();
	void OnMovementThresholdReached_Implementation();

protected:
	virtual void BeginPlay() override;
    
	bool bToolActive;
	FVector LastMovementDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Movement")
	float MovementThreshold;
    
	UPROPERTY(BlueprintReadOnly, Category = "Tool Movement")
	float AccumulatedDistance;
    
	FVector LastPosition;
};