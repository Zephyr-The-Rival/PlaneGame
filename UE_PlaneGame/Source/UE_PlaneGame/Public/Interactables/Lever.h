// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

UCLASS()
class UE_PLANEGAME_API ALever : public AInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Handle;
};
