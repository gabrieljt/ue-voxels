// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxels.h"
#include "Chunk.h"

UChunk::UChunk()
	: Width(3)
	, Depth(3)
	, Height(3)
	, Voxels()
{
	Voxels.Init(0, Width * Depth * Height);
}

UChunk::~UChunk()
{
}

void UChunk::SetVolume(int32 Width, int32 Depth, int32 Height)
{
	this->Width = Width;
	this->Depth = Depth;
	this->Height = Height;
	Voxels.Init(0, Width * Depth * Height);
}

int32 UChunk::GetVoxel(int32 I, int32 J, int32 K) const
{
	return int32();
}

void UChunk::SetVoxel(int32 I, int32 J, int32 K, const int32 Voxel)
{
	Voxels[Convert3DIndexToFlatIndex(I, J, K)] = Voxel;
}

void UChunk::AddVoxel(int32 I, int32 J, int32 K, const int32 VoxelType)
{
}

void UChunk::RemoveVoxel(int32 I, int32 J, int32 K)
{
}

int32 UChunk::Convert3DIndexToFlatIndex(int32 I, int32 J, int32 K) const
{
	return I + Width * (J + Depth * K);
}

bool UChunk::IsValidVoxelIndex(int32 I, int32 J, int32 K) const
{
	return true;
}

bool UChunk::IsVoxelOnChunkFace(const int32 I, const int32 J, const int32 K) const
{
	return true;
}

void UChunk::LogVoxels() const
{
	for (int32 I = 0; I < Width; ++I)
	{
		for (int32 J = 0; J < Depth; ++J)
		{
			for (int32 K = 0; K < Height; ++K)
			{
				UE_LOG(LogTemp, Warning, TEXT("UChunk::Voxels[%d,%d,%d]:%d"),
					I, J, K,
					Voxels[Convert3DIndexToFlatIndex(I, J, K)]
				)
			}
		}
	}
}