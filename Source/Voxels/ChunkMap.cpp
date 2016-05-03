// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxels.h"
#include "ChunkMap.h"

UChunkMap::UChunkMap()
	: FillMethod(EChunkFillMethod::CFM_Diagonal)
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
	: FillMethod(EChunkFillMethod::CFM_Diagonal)
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
				case EChunkFillMethod::CFM_Diagonal:
					Voxels[GetArrayIndex(I, J, K)] = I == J && J == K;
					break;
				case EChunkFillMethod::CFM_Hollow:
					Voxels[GetArrayIndex(I, J, K)] = ((I == 0 && J == 0) && K >= 0) || ((I == 0 && K == 0) && J >= 0) || ((J == 0 && K == 0) && I >= 0)
						|| ((I >= 0 && J >= 0) && K == Z - 1) || ((I >= 0 && K >= 0) && J == Y - 1) || ((J >= 0 && K >= 0) && I == X - 1) ? FMath::RandRange(0, VoxelTypesQuantity - 2) : VoxelTypesQuantity - 1;
					//I == 0 ||J  == 0 || K == 0 || I == X || J == Y || K == Z ? FMath::RandRange(0, VoxelTypesQuantity - 2) : VoxelTypesQuantity - 1;
					break;
				case EChunkFillMethod::CFM_Random:
					Voxels[GetArrayIndex(I, J, K)] = FMath::RandRange(0, VoxelTypesQuantity - 1);
					break;
				default:
					UE_LOG(LogTemp, Error, TEXT("EChunkFillMethod::%s not implemented"),
						*GetEnumValueToString<EChunkFillMethod>("EChunkFillMethod", FillMethod)
						)
						return;
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