// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Chunk.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChunkEvent);

UCLASS(ClassGroup = (Custom), meta = (Blueprint32SpawnableComponent))
class VOXELS_API UChunk : public UObject
{
	GENERATED_BODY()

public:
	UChunk();

	~UChunk();

	UPROPERTY(BlueprintAssignable)
		FChunkEvent OnAddVoxel;

	UPROPERTY(BlueprintAssignable)
		FChunkEvent OnRemoveVoxel;

	UFUNCTION(BlueprintCallable, Category = "Chunk Init")
		void SetVolume(int32 Width, int32 Depth, int32 Height);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UChunk Getters")
		int32 GetVoxel(int32 I, int32 J, int32 K) const;

	UFUNCTION(BlueprintCallable, Category = "UChunk Setters")
		void SetVoxel(int32 I, int32 J, int32 K, const int32 Voxel);

	UFUNCTION(BlueprintCallable, Category = "UChunk API")
		void AddVoxel(int32 I, int32 J, int32 K, const int32 VoxelType);

	UFUNCTION(BlueprintCallable, Category = "UChunk API")
		void RemoveVoxel(int32 I, int32 J, int32 K);

	// Extract
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voxels API")
		int32 Convert3DIndexToFlatIndex(int32 I, int32 J, int32 K) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UChunk API")
		bool IsValidVoxelIndex(int32 I, int32 J, int32 K) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UChunk API")
		bool IsVoxelOnChunkFace(const int32 I, const int32 J, const int32 K) const;

	UFUNCTION(BlueprintCallable, Category = "UChunk API")
		void LogVoxels() const;

private:
	UPROPERTY(VisibleAnywhere)
		int32 Width;

	UPROPERTY(VisibleAnywhere)
		int32 Depth;

	UPROPERTY(VisibleAnywhere)
		int32 Height;

	UPROPERTY(VisibleAnywhere)
		TArray<int32> Voxels;
};
