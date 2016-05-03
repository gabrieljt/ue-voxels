// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxels.h"
#include "ChunkMap.h"

UChunkMap::UChunkMap()
	: FillMethod(EChunkFillMethod::CFM_Random)
	, VoxelTypesQuantity(2)
	, X(3)
	, Y(3)
	, Z(3)
	, Voxels()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	Voxels.Init(0, X * Y * Z);
}

UChunkMap::UChunkMap(const int32 X, const int32 Y, const int32 Z)
	: FillMethod(EChunkFillMethod::CFM_Random)
	, VoxelTypesQuantity(2)
	, X(X)
	, Y(Y)
	, Z(Z)
	, Voxels()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	Voxels.Init(0, X * Y * Z);
}

void UChunkMap::BeginPlay()
{
	Super::BeginPlay();
	check(VoxelTypesQuantity > 1);
}

void UChunkMap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma region Public BP functions

void UChunkMap::SetVolume(int32 X, int32 Y, int32 Z)
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
	Voxels.Init(0, X * Y * Z);
}

void UChunkMap::GenerateChunk()
{
	for (int32 I = 0; I < X; ++I)
	{
		for (int32 J = 0; J < Y; ++J)
		{
			for (int32 K = 0; K < Z; ++K)
			{
				switch (FillMethod)
				{
				case EChunkFillMethod::CFM_Modulum:
					Voxels[GetArrayIndex(I, J, K)] = GetArrayIndex(I, J, K) % VoxelTypesQuantity == 0;
					break;
				case EChunkFillMethod::CFM_Random:
				default:
					Voxels[GetArrayIndex(I, J, K)] = FMath::RandRange(0, VoxelTypesQuantity - 1);
					break;
				}
			}
		}
	}
}

void UChunkMap::LogVoxels() const
{
	for (int32 I = 0; I < X; ++I)
	{
		for (int32 J = 0; J < Y; ++J)
		{
			for (int32 K = 0; K < Z; ++K)
			{
				UE_LOG(LogTemp, Warning, TEXT("Voxels[%d,%d,%d]:%d"),
					I, J, K,
					Voxels[GetArrayIndex(I, J, K)]
				)
			}
		}
	}
}

int32 UChunkMap::GetVoxelType(int32 I, int32 J, int32 K) const
{
	return Voxels[GetArrayIndex(I, J, K)];
}

EChunkFillMethod UChunkMap::GetChunkFillMethod() const
{
	return FillMethod;
}

int32 UChunkMap::GetVoxelTypesQuantity() const
{
	return VoxelTypesQuantity;
}

void UChunkMap::SetChunkFillMethod(EChunkFillMethod FillMethod)
{
	this->FillMethod = FillMethod;
}

void UChunkMap::SetVoxelTypesQuantity(int32 VoxelTypesQuantity)
{
	this->VoxelTypesQuantity = VoxelTypesQuantity;
}

#pragma endregion

#pragma region Private functions

int32 UChunkMap::GetArrayIndex(int32 I, int32 J, int32 K) const
{
	return I + X * (J + Y * K);
}

#pragma endregion