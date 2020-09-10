// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TileInstance.h"
#include "System/DerivedMatch3GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
//
//ATileInstance::ATileInstance()
//	:Super()
//{
//	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("COLLIDER"));
//	SelectedParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SELECTED"));
//	Collider->SetupAttachment(RootComponent);
//	SelectedParticleSystem->SetupAttachment(RootComponent);
//}

void ATileInstance::BeginPlay()
{
	Super::BeginPlay();
	CachedGameMode = Cast<ADerivedMatch3GameMode>(UGameplayStatics::GetGameMode(this));
}

void ATileInstance::OnMatched_Implementation(ETileMoveType::Type MoveType)
{
	FTimerHandle SpawnTimeHandler, CallOnMatchedHandler;
	FTimerDelegate DelegateSpawnedParticle, CallOnMathcedDelegate;

	switch (MoveType)
	{
	case ETileMoveType::MT_STANDARD:
		SetExplosionScale(.55f, .55f, .55f);
		ChecksTileType();
		break;
	case ETileMoveType::MT_MORE_TILES:
		SetExplosionScale(.65f, .65f, .65f);
		ChecksTileType();
		break;
	case ETileMoveType::MT_COMBO:
		SetExplosionScale(.75f, .75f, .75f);
		ChecksTileType();
		break;
	case ETileMoveType::MT_BOMB:
		SetExplosionScale(.75f, .75f, .75f);
		break;
	case ETileMoveType::MT_ALL_THE_BOMBS:
		SetExplosionScale(1, 1, 1);
		break;
	}

	CachedGameMode->SpawnScoreDisplay(GetActorLocation() + FVector::RightVector * 10);

	DelegateSpawnedParticle.BindLambda([this](){
		if (this->ParticleSystem != nullptr) {
			UParticleSystemComponent* SpawnedParticle =	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, GetActorLocation() + FVector::RightVector * 10, FRotator::ZeroRotator, true, EPSCPoolMethod::None, true);
			TileSpecificParticles(SpawnedParticle);
		}
	});

	CallOnMathcedDelegate.BindLambda([this](ETileMoveType::Type Param) { Super::OnMatched_Implementation(Param); }, MoveType);
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandler, DelegateSpawnedParticle, FMath::FRandRange(0, .2f), false);
	GetWorld()->GetTimerManager().SetTimer(CallOnMatchedHandler, CallOnMathcedDelegate, 0.2f, false);
}

void ATileInstance::ChecksTileType()
{
	switch (Super::GetTileTypeID())
	{
	case 4:
		CachedGameMode->MatchBombsAchievement();
		break;
	case 5:
		CachedGameMode->MatchBlockAchievement();
		break;
	}
}

void ATileInstance::TileSpecificParticles(UParticleSystemComponent* SpawnedParticle)
{
	if (SpawnedParticle != nullptr)
	{
		SpawnedParticle->SetTranslucentSortPriority(50);
		SpawnedParticle->SetWorldScale3D(ExplosionScale);
	}
}