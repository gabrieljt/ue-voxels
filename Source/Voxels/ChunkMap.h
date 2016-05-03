// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "ChunkMap.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VOXELS_API UChunkMap : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChunkMap();

	UChunkMap(const int, const int, const int);

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GenerateRandomChunk();
private:
	TArray<int> Voxels;

	UPROPERTY(VisibleAnywhere)
		int X;

	UPROPERTY(VisibleAnywhere)
		int Y;

	UPROPERTY(VisibleAnywhere)
		int Z;

	void LogVoxels() const;

	int GetArrayIndex(int i, int j, int k) const;
};
