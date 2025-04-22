#pragma once

#include <Interactables/Items/WorldItem.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Items/WorldItem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "WobbleWater.generated.h"

UCLASS()
class UE_PLANEGAME_API AWobbleWater :AWorldItem
{
	GENERATED_BODY()
    
public:    

	AWobbleWater();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
    
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Wobble")
	float MaxWobble = 0.03f;
    
	UPROPERTY(EditAnywhere, Category = "Wobble")
	float WobbleSpeed = 1.0f;
    
	UPROPERTY(EditAnywhere, Category = "Wobble")
	float Recovery = 1.0f;
	
	FVector LastPosition;
	FVector Velocity;
	FRotator LastRotation;
	FVector AngularVelocity;
    
	float WobbleAmountX;
	float WobbleAmountZ;
	float WobbleAmountToAddX;
	float WobbleAmountToAddZ;
	float Pulse;
	float Time = 0.5f;
};
