// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxels.h"
#include "ChunkMap.h"

UChunkMap::UChunkMap()
	: X(3)
	, Y(3)
	, Z(3)
	, Voxels()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	Voxels.Init(0, X * Y * Z);
}

UChunkMap::UChunkMap(const int32 X, const int32 Y, const int32 Z)
	: X(X)
	, Y(Y)
	, Z(Z)
	, Voxels()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	Voxels.Init(0, X * Y * Z);
}

// Called when the game starts
void UChunkMap::BeginPlay()
{
	Super::BeginPlay();
	//LogVoxels();
}

void UChunkMap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UChunkMap::GenerateRandomChunk()
{
	for (int32 i = 0; i < X; ++i)
	{
		for (int32 j = 0; j < Y; ++j)
		{
			for (int32 k = 0; k < Z; ++k)
			{
				Voxels[GetArrayIndex(i, j, k)] = GetArrayIndex(i, j, k) % 2 == 0;
			}
		}
	}
}

void UChunkMap::LogVoxels() const
{
	for (int32 i = 0; i < X; ++i)
	{
		for (int32 j = 0; j < Y; ++j)
		{
			for (int32 k = 0; k < Z; ++k)
			{
				UE_LOG(LogTemp, Warning, TEXT("Voxels[%d,%d,%d]:%d"),
					i, j, k,
					Voxels[GetArrayIndex(i, j, k)]
				)
			}
		}
	}
}

int32 UChunkMap::GetVoxelType(int32 i, int32 j, int32 k)
{
	return Voxels[GetArrayIndex(i, j, k)];
}

void UChunkMap::SetVolume(int32 X, int32 Y, int32 Z)
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
	Voxels.Init(0, X * Y * Z);
}

int32 UChunkMap::GetArrayIndex(int32 i, int32 j, int32 k) const
{
	return i + X * (j + Y * k);
}