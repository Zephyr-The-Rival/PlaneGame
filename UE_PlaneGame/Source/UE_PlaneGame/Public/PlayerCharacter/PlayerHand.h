// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerHand.generated.h"

class AWorldItem;
class USphereComponent;

UCLASS()
class UE_PLANEGAME_API APlayerHand : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerHand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* HandCollision;

public:
	AWorldItem* GetOverlappingItem();
};
