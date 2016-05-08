// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "ChunkMap.generated.h"

UENUM(BlueprintType)
enum class EChunkPattern : uint8
{
	CP_Empty			UMETA(DisplayName = "Empty"),
	CP_Modulum			UMETA(DisplayName = "Modulum"),
	CP_Plane			UMETA(DisplayName = "Plane"),
	CP_Hollow			UMETA(DisplayName = "Hollow"),
	CP_Random			UMETA(DisplayName = "Random"),
	CP_Random_Hollow	UMETA(DisplayName = "Random Hollow"),
	CP_Noise			UMETA(DisplayName = "Noise")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVoxelEvent);

UCLASS(ClassGroup = (Custom), meta = (Blueprint32SpawnableComponent))
class VOXELS_API UChunkMap : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChunkMap();

	UChunkMap(const int32, const int32, const int32);

	UPROPERTY(BlueprintAssignable)
		FVoxelEvent OnAddVoxel;

	UPROPERTY(BlueprintAssignable)
		FVoxelEvent OnRemoveVoxel;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Chunk Init")
		void SetVolume(int32 Width, int32 Depth, int32 Height);

	UFUNCTION(BlueprintCallable, Category = "Chunk Init")
		void GenerateChunk();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Getters")
		int32 GetVoxel(int32 I, int32 J, int32 K) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Getters")
		EChunkPattern GetChunkPattern() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk Getters")
		int32 GetVoxelTypes() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk Setters")
		void SetVoxel(int32 I, int32 J, int32 K, const int32 Voxel);

	UFUNCTION(BlueprintCallable, Category = "Chunk API")
		void AddVoxel(int32 I, int32 J, int32 K, const int32 VoxelType);

	UFUNCTION(BlueprintCallable, Category = "Chunk API")
		void RemoveVoxel(int32 I, int32 J, int32 K);

	UFUNCTION(BlueprintCallable, Category = "Chunk Setters")
		void SetChunkPattern(EChunkPattern Pattern);

	UFUNCTION(BlueprintCallable, Category = "Chunk Setters")
		void SetVoxelTypes(int32 VoxelTypes);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk API")
		int32 GetArrayIndex(int32 I, int32 J, int32 K) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chunk API")
		bool IsValidIndex(int32 I, int32 J, int32 K) const;

	UFUNCTION(BlueprintCallable, Category = "Chunk API")
		void LogVoxels() const;

private:
	UPROPERTY(VisibleAnywhere)
		EChunkPattern Pattern;

	UPROPERTY(VisibleAnywhere)
		int32 VoxelTypes;

	UPROPERTY(VisibleAnywhere)
		int32 Width;

	UPROPERTY(VisibleAnywhere)
		int32 Depth;

	UPROPERTY(VisibleAnywhere)
		int32 Height;

	UPROPERTY(VisibleAnywhere)
		TArray<int32> Voxels;

	int32 GetModulumPatternValue(const int32 I, const int32 J, const int32 K) const;

	int32 GetPlanePatternValue(const int32 I, const int32 J, const int32 K) const;

	int32 GetHollowPatternValue(const int32 I, const int32 J, const int32 K) const;

	int32 GetRandomHollowPatternValue(const int32 I, const int32 J, const int32 K) const;

	int32 GetEmptyVoxel() const;

	int32 GetRandomVoxel() const;

	int32 GetRandomSolidVoxel() const;

	bool IsBorder(const int32 I, const int32 J, const int32 K) const;

	void GenerateValue(const int32 I, const int32 J, const int32 K);

	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueToString(const FString& Name, TEnum Value);
};
