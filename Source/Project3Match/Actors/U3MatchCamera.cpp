// Fill out your copyright notice in the Description page of Project Settings.


#include "U3MatchCamera.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "../../../../../../Engine/Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteActor.h"

AU3MatchCamera::AU3MatchCamera():Super(FObjectInitializer())
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

	// 블루 프린트 재 분석 및 적용
	//if (Viewport.X > 0)
	//{
	//	float Ratio = (DefaultFit.Y / DefaultFit.X) / (Viewport.Y / Viewport.X);
	//	
	//	if (FMath::IsNearlyEqual(Viewport.X / Viewport.Y * 1.0, 0.75, 0.01)) // 4 : 3
	//	{
	//		ACameraActor::GetCameraComponent()->SetOrthoWidth(Ratio * 1 * DesiredSize);
	//		bIsCameraSet = true;
	//	}
	//	else
	//	{
	//		if (FMath::IsNearlyEqual(Viewport.X / Viewport.Y * 1.0, 0.48, 0.035))
	//		{
	//			if (::IsValid(GridBorder))
	//			{
	//				GetCameraComponent()->SetWorldLocation(MoveCameraZ(), false);
	//			}
	//		}
	//		else
	//		{
	//			if (FMath::IsNearlyEqual(Viewport.X / Viewport.Y * 1.0, 0.48, 0.035))
	//			{

	//			}
	//			else
	//			{

	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	// SetTimerByEvent(.2, &UpdateCamera) 등록
	//	// UpdateCamera() 함수 재호출
	//}
}

void AU3MatchCamera::CheckViewport()
{

}

FVector AU3MatchCamera::MoveCameraZ()
{
	return FVector();
}