// Fill out your copyright notice in the Description page of Project Settings.


#include "U3MatchCamera.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "../../../../../../Engine/Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteActor.h"

AU3MatchCamera::AU3MatchCamera():Super(FObjectInitializer()),
DefaultFit(FVector2D(640, 1136)), DesiredSize(256)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AU3MatchCamera::BeginPlay()
{
	Super::BeginPlay();
	//APaperSpriteActor* Actor = nullptr;
}

void AU3MatchCamera::UpdateCamera()
{
	FVector2D Viewport(0, 0);
	if(UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport())
		ViewportClient->GetViewportSize(Viewport);

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
	{

	}
	else
	{

	}
}

FVector AU3MatchCamera::MoveCameraZ()
{
	return FVector();
}