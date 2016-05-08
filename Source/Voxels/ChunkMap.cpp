// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxels.h"
#include "ChunkMap.h"

UChunkMap::UChunkMap()
	: Pattern(EChunkPattern::CP_Empty)
	, VoxelTypes(2) // Default Solid and Empty Types (0, 1)
	, Width(3)
	, Depth(3)
	, Height(3)
	, Voxels()
	, Chunks()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	Voxels.Init(0, Width * Depth * Height);
}

UChunkMap::UChunkMap(const int32 Width, const int32 Depth, const int32 Height)
	: Pattern(EChunkPattern::CP_Empty)
	, VoxelTypes(2)
	, Width(Width)
	, Depth(Depth)
	, Height(Height)
	, Voxels()
	, Chunks()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	Voxels.Init(0, Width * Depth * Height);
}

void UChunkMap::BeginPlay()
{
	Super::BeginPlay();
	check(VoxelTypes > 1);
}

void UChunkMap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma region Public BP functions

void UChunkMap::SetVolume(int32 Width, int32 Depth, int32 Height)
{
	this->Width = Width;
	this->Depth = Depth;
	this->Height = Height;
	Voxels.Init(0, Width * Depth * Height);
}

void UChunkMap::GenerateChunk()
{
	Chunks.Add(FIntVector(0, 0, 0), NewObject<UChunk>());
	Chunks[FIntVector(0, 0, 0)]->SetVolume(Width, Depth, Height);
	for (int32 I = 0; I < Width; ++I)
	{
		for (int32 J = 0; J < Depth; ++J)
		{
			for (int32 K = 0; K < Height; ++K)
			{
				GenerateValue(I, J, K);
				Chunks[FIntVector(0, 0, 0)]->SetVoxel(I, J, K, Voxels[GetArrayIndex(I, J, K)]);
			}
		}
	}
	Chunks[FIntVector(0, 0, 0)]->LogVoxels();
}

int32 UChunkMap::GetVoxel(int32 I, int32 J, int32 K) const
{
	return Voxels[GetArrayIndex(I, J, K)];
}

EChunkPattern UChunkMap::GetChunkPattern() const
{
	return Pattern;
}

int32 UChunkMap::GetVoxelTypes() const
{
	return VoxelTypes;
}

void UChunkMap::SetVoxel(int32 I, int32 J, int32 K, const int32 Voxel)
{
	Voxels[GetArrayIndex(I, J, K)] = Voxel;
}

void UChunkMap::AddVoxel(int32 I, int32 J, int32 K, const int32 VoxelType)
{
	SetVoxel(I, J, K, VoxelType);
	OnAddVoxel.Broadcast();
}

void UChunkMap::RemoveVoxel(int32 I, int32 J, int32 K)
{
	SetVoxel(I, J, K, GetEmptyVoxel());
	OnRemoveVoxel.Broadcast();
}

void UChunkMap::SetChunkPattern(EChunkPattern Pattern)
{
	this->Pattern = Pattern;
}

void UChunkMap::SetVoxelTypes(int32 VoxelTypes)
{
	this->VoxelTypes = VoxelTypes;
}

int32 UChunkMap::GetArrayIndex(int32 I, int32 J, int32 K) const
{
	return I + Width * (J + Depth * K);
}

bool UChunkMap::IsValidIndex(int32 I, int32 J, int32 K) const
{
	return (I >= 0 && I < Width) && (J >= 0 && J < Depth) && (K >= 0 && K < Height);
}

void UChunkMap::LogVoxels() const
{
	for (int32 I = 0; I < Width; ++I)
	{
		for (int32 J = 0; J < Depth; ++J)
		{
			for (int32 K = 0; K < Height; ++K)
			{
				UE_LOG(LogTemp, Warning, TEXT("Voxels[%d,%d,%d]:%d"),
					I, J, K,
					Voxels[GetArrayIndex(I, J, K)]
				)
			}
		}
	}
}

#pragma endregion

#pragma region Private functions

int32 UChunkMap::GetModulumPatternValue(const int32 I, const int32 J, const int32 K) const
{
	return GetArrayIndex(I, J, K) % VoxelTypes == 0;
}

int32 UChunkMap::GetPlanePatternValue(const int32 I, const int32 J, const int32 K) const
{
	return  K < 1 ? GetRandomSolidVoxel() : GetEmptyVoxel();
}

int32 UChunkMap::GetHollowPatternValue(const int32 I, const int32 J, const int32 K) const
{
	return  IsBorder(I, J, K) ? GetRandomSolidVoxel() : GetEmptyVoxel();
}

int32 UChunkMap::GetRandomHollowPatternValue(const int32 I, const int32 J, const int32 K) const
{
	return  GetHollowPatternValue(I, J, K) < GetEmptyVoxel() ? GetRandomVoxel() : GetEmptyVoxel();
}

int32 UChunkMap::GetEmptyVoxel() const
{
	return  VoxelTypes - 1;
}

int32 UChunkMap::GetRandomVoxel() const
{
	return  FMath::RandRange(0, GetEmptyVoxel());
}

int32 UChunkMap::GetRandomSolidVoxel() const
{
	return FMath::Clamp(FMath::RandRange(0, GetEmptyVoxel()) - 1, 0, VoxelTypes - 2);
}

bool UChunkMap::IsBorder(const int32 I, const int32 J, const int32 K) const
{
	return  (I == 0 && (J >= 0 && K >= 0)) || (I == 0 && (J >= 0 && K >= 0)) || (J == 0 && (I >= 0 && K == 0))
		|| ((I >= 0 && J >= 0) && K == Height - 1) || ((I >= 0 && K >= 0) && J == Depth - 1) || ((J >= 0 && K >= 0) && I == Width - 1)
		|| ((I <= Width - 1 && J <= Depth - 1) && K == 0) || ((I <= Width - 1 && K <= Height - 1) && J == 0) || ((J <= Depth - 1 && K <= Height - 1) && I == 0);
}

void UChunkMap::GenerateValue(const int32 I, const int32 J, const int32 K)
{
	switch (Pattern)
	{
	case EChunkPattern::CP_Empty:
		Voxels[GetArrayIndex(I, J, K)] = GetEmptyVoxel();
		break;
	case EChunkPattern::CP_Modulum:
		Voxels[GetArrayIndex(I, J, K)] = GetModulumPatternValue(I, J, K);
		break;
	case EChunkPattern::CP_Plane:
		Voxels[GetArrayIndex(I, J, K)] = GetPlanePatternValue(I, J, K);
		break;
	case EChunkPattern::CP_Hollow:
		Voxels[GetArrayIndex(I, J, K)] = GetHollowPatternValue(I, J, K);
		break;
	case EChunkPattern::CP_Random_Hollow:
		Voxels[GetArrayIndex(I, J, K)] = GetRandomHollowPatternValue(I, J, K);
		break;
	case EChunkPattern::CP_Random:
		Voxels[GetArrayIndex(I, J, K)] = GetRandomVoxel();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("EChunkPattern::%s not implemented"),
			*GetEnumValueToString<EChunkPattern>("EChunkPattern", Pattern)
		)
			checkNoEntry();
		break;
	}
}

template<typename TEnum>
FString UChunkMap::GetEnumValueToString(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}

	return enumPtr->GetEnumName((int32)Value);
}

#pragma endregion