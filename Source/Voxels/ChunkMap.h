// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "ChunkMap.generated.h"

UENUM(BlueprintType)
enum class EChunkFillMethod : uint8
{
	CFM_Modulum 	UMETA(DisplayName = "Modulum"),
	CFM_Random 	UMETA(DisplayName = "Random"),
	CFM_Noise	UMETA(DisplayName = "Noise")
};

UCLASS(ClassGroup = (Custom), meta = (Blueprint32SpawnableComponent))
class VOXELS_API UChunkMap : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChunkMap();

	UChunkMap(const int32, const int32, const int32);

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Chunk Generation")
		void SetVolume(int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Category = "Chunk Generation")
		void GenerateChunk();

	UFUNCTION(BlueprintCallable, Category = "Chunk Utils")
		void LogVoxels() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Getters")
		int32 GetVoxelType(int32 I, int32 J, int32 K) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Getters")
		EChunkFillMethod GetChunkFillMethod() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Getters")
		int32 GetVoxelTypesQuantity() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk Setters")
		void SetChunkFillMethod(EChunkFillMethod FillMethod);

	UFUNCTION(BlueprintCallable, Category = "Chunk Setters")
		void SetVoxelTypesQuantity(int32 VoxelTypesQuantity);

private:
	UPROPERTY(VisibleAnywhere)
		EChunkFillMethod FillMethod;

	UPROPERTY(VisibleAnywhere)
		int32 VoxelTypesQuantity;

	UPROPERTY(VisibleAnywhere)
		int32 X;

	UPROPERTY(VisibleAnywhere)
		int32 Y;

	UPROPERTY(VisibleAnywhere)
		int32 Z;

	UPROPERTY(VisibleAnywhere)
		TArray<int32> Voxels;

	int32 GetArrayIndex(int32 I, int32 J, int32 K) const;
};
