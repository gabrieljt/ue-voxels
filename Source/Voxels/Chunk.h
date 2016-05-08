// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Chunk.generated.h"

UCLASS(ClassGroup = (Custom), meta = (Blueprint32SpawnableComponent))
class VOXELS_API UChunk : public USceneComponent
{
	GENERATED_BODY()

public:
	UChunk();

	UChunk(const int32, const int32, const int32);

	~UChunk();

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
