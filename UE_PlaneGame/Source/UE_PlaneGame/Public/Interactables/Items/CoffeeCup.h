// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldItem.h"
#include "GameFramework/Actor.h"
#include "CoffeeCup.generated.h"

UCLASS()
class ACoffeeCup : public AWorldItem
{
	GENERATED_BODY()
	
public:	

	ACoffeeCup();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* CupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* LiquidPlane;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Rotation")
	float LiquidDampingFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Rotation")
	float LiquidTiltResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Rotation")
	float LiquidSmoothness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Rotation")
	float MaxLiquidAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Rotation")
	float InertiaFactor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Motion")
	float MotionResponseFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Motion")
	float MotionDampingFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Motion")
	float MotionSmoothness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Motion")
	float MinSpeedThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Motion")
	float MaxMotionAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid Physics|Motion")
	bool EnableMotionEffects;

private:
	FVector CurrentLiquidRotation;
	FVector TargetLiquidRotation;
	FVector PrevCupRotation;
	
	FVector PrevLocation;
	FVector CurrentVelocity;
	FVector PrevVelocity;
	FVector MotionOffset;
	FVector TargetMotionOffset;
	
	void InitializeLiquidPlane();
	void CalculateLiquidRotation(float DeltaTime);
	void CalculateMotionEffects(float DeltaTime);
	void UpdateLiquidTransform();
};