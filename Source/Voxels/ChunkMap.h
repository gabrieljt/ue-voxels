// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "ChunkMap.generated.h"

UCLASS(ClassGroup = (Custom), meta = (Blueprint32SpawnableComponent))
class VOXELS_API UChunkMap : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChunkMap();

	UChunkMap(const int32, const int32, const int32);

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Chunk Generation")
		void SetVolume(int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Category = "Chunk Generation")
		void GenerateRandomChunk();

	UFUNCTION(BlueprintCallable, Category = "Chunk Utils")
		void LogVoxels() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Utils")
		int32 GetVoxelType(int32 i, int32 j, int32 k) const;

private:
	TArray<int32> Voxels;

	UPROPERTY(VisibleAnywhere)
		int32 X;

	UPROPERTY(VisibleAnywhere)
		int32 Y;

	UPROPERTY(VisibleAnywhere)
		int32 Z;

	int32 GetArrayIndex(int32 i, int32 j, int32 k) const;
};
