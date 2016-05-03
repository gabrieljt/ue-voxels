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

UChunkMap::UChunkMap(const int X, const int Y, const int Z)
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

	LogVoxels();
}

void UChunkMap::LogVoxels() const
{
	for (int i = 0; i < X; ++i)
	{
		for (int j = 0; j < Y; ++j)
		{
			for (int k = 0; k < Z; ++k)
			{
				UE_LOG(LogTemp, Warning, TEXT("Voxels[%d,%d,%d]:%d"),
					i, j, k,
					Voxels[GetArrayIndex(i, j, k)]
				)
			}
		}
	}
}

int UChunkMap::GetArrayIndex(int i, int j, int k) const
{
	return i + X * (j + Y * k);
}

// Called every frame
void UChunkMap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UChunkMap::GenerateRandomChunk()
{
	for (int i = 0; i < X; ++i)
	{
		for (int j = 0; j < Y; ++j)
		{
			for (int k = 0; k < Z; ++k)
			{
				Voxels[GetArrayIndex(i, j, k)] = 1;
			}
		}
	}
}