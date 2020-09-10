// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Tile.h"
#include "TileInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API ATileInstance : public ATile
{
	GENERATED_BODY()
public:
	//ATileInstance();
	virtual void BeginPlay() override;
	virtual void OnMatched_Implementation(ETileMoveType::Type MoveType) override;

protected:
	UFUNCTION()
		virtual void TileSpecificParticles(class UParticleSystemComponent* SpawnedParticle);
	void SetExplosionScale(float X, float Y, float Z) { ExplosionScale = FVector(X, Y, Z); }
	void ChecksTileType();
	
protected:
	/*UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Collider;
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* SelectedParticle;*/
	/*UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* SelectedParticleSystem;*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		class ADerivedMatch3GameMode* CachedGameMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		class UParticleSystem* ParticleSystem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		FVector ExplosionScale;
};
