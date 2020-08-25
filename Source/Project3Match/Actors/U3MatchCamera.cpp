// Fill out your copyright notice in the Description page of Project Settings.



#include "U3MatchCamera.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "../UObjects/GlobalGameInstance.h"


#include "../../../../../../Engine/Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteActor.h"

AU3MatchCamera::AU3MatchCamera():Super(FObjectInitializer()),
DefaultFit(FVector2D(640, 1136)), DesiredSize(256)
{
	PrimaryActorTick.bCanEverTick = false;
	using HelperClasses::FDoOnceObject;
	DoOnce = new FDoOnceObject();
}

void AU3MatchCamera::BeginPlay()
{
	Super::BeginPlay();
}

void AU3MatchCamera::BeginDestroy()
{
	Super::BeginDestroy();
	
	if(DoOnce!= nullptr)
		delete DoOnce;
}

FVector2D GetViewport(UWorld* World)
{
	if (World == nullptr)
		return FVector2D(1, 1);

	FVector2D Viewport;
	UGameViewportClient* GameViewport = World->GetGameViewport();
	GameViewport->GetViewportSize(Viewport);
	return Viewport;
}

void AU3MatchCamera::UpdateCamera()
{
	FVector2D Viewport = GetViewport(GetWorld());
	
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
				SetCameraSet(true);
			}
			else
			{
				Super::GetCameraComponent()->SetOrthoWidth(DefaultFitRatio / ViewPortRatio * DesiredSize);
				SetCameraSet(true);
			}
		}
	}
	else
	{
		// SetTimerByEvent(.2, &UpdateCamera) µî·Ï
		FTimerHandle TimeHandler;
		GetWorldTimerManager().SetTimer(TimeHandler, this, &AU3MatchCamera::UpdateCamera, 2, false);
	}
}

void AU3MatchCamera::CheckViewport()
{
	if (::IsValid(GlobalGameInstance))
		GlobalGameInstance = AActor::GetGameInstance();

	FVector2D Viewport = GetViewport(GetWorld());
	if (Viewport.X != StoreSize.X || Viewport.Y != StoreSize.Y)
	{
		StoreSize = Viewport;
		UGlobalGameInstance* GameInstance = static_cast<UGlobalGameInstance*>(GlobalGameInstance);
		GameInstance->SetCameraResize(StoreSize);

		if (DoOnce)
		{
			UpdateCamera();
			
		}
	}

}

FVector AU3MatchCamera::MoveCameraZ()
{
	//APaperSpriteActor* Actor = nullptr;
	return FVector();
}