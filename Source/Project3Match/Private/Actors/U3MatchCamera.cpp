// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/U3MatchCamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "System/GlobalGameInstance.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "../../../../../../Engine/Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteActor.h"

AU3MatchCamera::AU3MatchCamera():Super(FObjectInitializer()),
DefaultFit(FVector2D(640, 1136)), DesiredSize(256)
{
	PrimaryActorTick.bCanEverTick = false;
	GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
}

void AU3MatchCamera::BeginPlay()
{
	Super::BeginPlay();

	GlobalGameInstance = Cast<UGlobalGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void AU3MatchCamera::BeginDestroy()
{
	Super::BeginDestroy();
}

void AU3MatchCamera::UpdateCamera()
{
	FVector2D Viewport = UWidgetLayoutLibrary::GetViewportSize(this);
	
	if (Viewport.X > 0)
	{
		double ViewPortRatio = Viewport.Y / Viewport.X;
		double DefaultFitRatio = DefaultFit.Y / DefaultFit.X;
		if (FMath::IsNearlyEqual(1.0 * Viewport.X / Viewport.Y, .75, .01))
		{
			Super::GetCameraComponent()->SetOrthoWidth(DefaultFitRatio / ViewPortRatio * 1.0f);
			SetCameraSet(true);
		}
		else
		{
			if(FMath::IsNearlyEqual(1.0 * Viewport.X / Viewport.Y, 0.48, 0.035)||
				FMath::IsNearlyEqual(1.0 * Viewport.X / Viewport.Y, 0.48, 0.038))
			{
				if (::IsValid(GridBorder))
					Super::GetCameraComponent()->SetWorldLocation(MoveCameraZ());
				GetCameraComponent()->SetOrthoWidth((DefaultFitRatio / ViewPortRatio) * 1.28f * DesiredSize);
			}
			else
			{
				float Ortho = DefaultFitRatio / ViewPortRatio * DesiredSize;
				UE_LOG(LogTemp, Warning, TEXT("Camera Ortho %f"), Ortho);
				Super::GetCameraComponent()->SetOrthoWidth(Ortho);
			}
				
			SetCameraSet(true);
		}
	}
	else
	{
		// SetTimerByEvent(.2, &UpdateCamera) µî·Ï
		GetWorldTimerManager().SetTimer(TimeHandler, this, &AU3MatchCamera::UpdateCamera, .2f, false);
	}
}

void AU3MatchCamera::CheckViewport()
{
	if (::IsValid(GlobalGameInstance))
		GlobalGameInstance = Cast<UGlobalGameInstance>(AActor::GetGameInstance());

	FVector2D Viewport = UWidgetLayoutLibrary::GetViewportSize(this);
	if (Viewport.X != StoreSize.X || Viewport.Y != StoreSize.Y)
	{
		StoreSize = Viewport;
		GlobalGameInstance->SetCameraResize(StoreSize);
		GlobalGameInstance->StoredCameraRez = StoreSize;
		
		if (DoOnce)
		{
			UpdateCamera();
			GlobalGameInstance->GameScreenResize();
			return;
		}

		DoOnce.Reset();
	}

	if ((Viewport.X != 0 && Viewport.Y != 0) && DoOnce)
	{
		UpdateCamera();
		GlobalGameInstance->GameScreenResize();
		return;
	}

	GlobalGameInstance->GameScreenResize();
}

FVector AU3MatchCamera::MoveCameraZ()
{
	FVector WorldLocation = GetCameraComponent()->K2_GetComponentLocation();
	FVector Origin, Extend;
	GridBorder->GetActorBounds(false, Origin, Extend);
	WorldLocation.Z = (WorldLocation.Z - Origin.Z - Extend.Z) + WorldLocation.Z;
	
	return WorldLocation;
}