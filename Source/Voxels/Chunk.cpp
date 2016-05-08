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

UChunk::UChunk(const int32 Width, const int32 Depth, const int32 Height)
	: Width(Width)
	, Depth(Depth)
	, Height(Height)
	, Voxels()
{

	Voxels.Init(0, Width * Depth * Height);
}

UChunk::~UChunk()
{
}
