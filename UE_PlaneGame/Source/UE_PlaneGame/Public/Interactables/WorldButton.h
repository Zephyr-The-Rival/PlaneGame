// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "WorldButton.generated.h"

class APlanePlayerCharacter;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonEvent,APlanePlayerCharacter*, Player);
UCLASS()
class UE_PLANEGAME_API AWorldButton : public AInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ButtonBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* InteractCollision;
public:

	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintCallable)
	FButtonEvent OnButtonPressed;

	UFUNCTION(BlueprintNativeEvent)
	void Press(APlanePlayerCharacter* Player);
	void Press_Implementation(APlanePlayerCharacter* Player);
};
