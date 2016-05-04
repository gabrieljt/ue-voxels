// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxels.h"
#include "ChunkMap.h"

UChunkMap::UChunkMap()
	: Pattern(EChunkPattern::CP_Diagonal)
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
	: Pattern(EChunkPattern::CP_Diagonal)
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
				GenerateValue(I, J, K);
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

EChunkPattern UChunkMap::GetChunkFillMethod() const
{
	return Pattern;
}

int32 UChunkMap::GetVoxelTypesQuantity() const
{
	return VoxelTypesQuantity;
}

void UChunkMap::SetVoxel(const int32 I, const int32 J, const int32 K, const int32 Voxel)
{
	Voxels[GetArrayIndex(I, J, K)] = Voxel;
}

void UChunkMap::SetChunkPattern(EChunkPattern FillMethod)
{
	this->Pattern = FillMethod;
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

int32 UChunkMap::GetModulumFillMethodValue(const int32 I, const int32 J, const int32 K) const
{
	return GetArrayIndex(I, J, K) % VoxelTypesQuantity == 0;
}

int32 UChunkMap::GetDiagonalFillMethodValue(const int32 I, const int32 J, const int32 K) const
{
	return  I == J || I == K || J == K;
}

int32 UChunkMap::GetHollowFillMethodValue(const int32 I, const int32 J, const int32 K) const
{
	return  IsBorder(I, J, K) ? FMath::RandRange(0, VoxelTypesQuantity - 2) : VoxelTypesQuantity - 1;
}

int32 UChunkMap::GetRandomFillMethodValue() const
{
	return  FMath::RandRange(0, VoxelTypesQuantity - 1);
}

int32 UChunkMap::GetRandomHollowFillMethodValue(const int32 I, const int32 J, const int32 K) const
{
	return  GetHollowFillMethodValue(I, J, K) < VoxelTypesQuantity - 1 ? GetRandomFillMethodValue() : VoxelTypesQuantity - 1;
}

bool UChunkMap::IsBorder(const int32 I, const int32 J, const int32 K) const
{
	return  (I == 0 && (J >= 0 && K >= 0)) || (I == 0 && (J >= 0 && K >= 0)) || (J == 0 && (I >= 0 && K == 0))
		|| ((I >= 0 && J >= 0) && K == Z - 1) || ((I >= 0 && K >= 0) && J == Y - 1) || ((J >= 0 && K >= 0) && I == X - 1)
		|| ((I <= X - 1 && J <= Y - 1) && K == 0) || ((I <= X - 1 && K <= Z - 1) && J == 0) || ((J <= Y - 1 && K <= Z - 1) && I == 0);
}

void UChunkMap::GenerateValue(const int32 I, const int32 J, const int32 K)
{
	switch (Pattern)
	{
	case EChunkPattern::CP_Modulum:
		Voxels[GetArrayIndex(I, J, K)] = GetModulumFillMethodValue(I, J, K);
		break;
	case EChunkPattern::CP_Diagonal:
		Voxels[GetArrayIndex(I, J, K)] = GetDiagonalFillMethodValue(I, J, K);
		break;
	case EChunkPattern::CP_Hollow:
		Voxels[GetArrayIndex(I, J, K)] = GetHollowFillMethodValue(I, J, K);
		break;
	case EChunkPattern::CP_Random_Hollow:
		Voxels[GetArrayIndex(I, J, K)] = GetRandomHollowFillMethodValue(I, J, K);
		break;
	case EChunkPattern::CP_Random:
		Voxels[GetArrayIndex(I, J, K)] = GetRandomFillMethodValue();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("EChunkFillMethod::%s not implemented"),
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