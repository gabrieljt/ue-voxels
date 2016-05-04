// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "ChunkMap.generated.h"

UENUM(BlueprintType)
enum class EChunkPattern : uint8
{
	CP_Modulum			UMETA(DisplayName = "Modulum"),
	CP_Diagonal		UMETA(DisplayName = "Diagonal"),
	CP_Hollow			UMETA(DisplayName = "Hollow"),
	CP_Random			UMETA(DisplayName = "Random"),
	CP_Random_Hollow	UMETA(DisplayName = "Random Hollow"),
	CP_Noise			UMETA(DisplayName = "Noise")
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
		int32 GetVoxel(int32 I, int32 J, int32 K) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Getters")
		EChunkPattern GetChunkPattern() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Getters")
		int32 GetVoxelTypes() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk Setters")
		void SetVoxel(const int32 I, const int32 J, const int32 K, const int32 Voxel);

	UFUNCTION(BlueprintCallable, Category = "Chunk Setters")
		void SetChunkPattern(EChunkPattern Pattern);

	UFUNCTION(BlueprintCallable, Category = "Chunk Setters")
		void SetVoxelTypes(int32 VoxelTypesQuantity);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utils")
		int32 GetArrayIndex(int32 I, int32 J, int32 K) const;

private:
	UPROPERTY(VisibleAnywhere)
		EChunkPattern Pattern;

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

	int32 GetModulumPatternValue(const int32 I, const int32 J, const int32 K) const;

	int32 GetDiagonalPatternValue(const int32 I, const int32 J, const int32 K) const;

	int32 GetHollowPatternValue(const int32 I, const int32 J, const int32 K) const;

	int32 GetRandomPatternValue() const;

	int32 GetRandomHollowPatternValue(const int32 I, const int32 J, const int32 K) const;

	bool IsBorder(const int32 I, const int32 J, const int32 K) const;

	void GenerateValue(const int32 I, const int32 J, const int32 K);

	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueToString(const FString& Name, TEnum Value);
};
