
#include "System/Match3PlayerController.h"
#include "Kismet/GameplayStatics.h"

AMatch3PlayerController::AMatch3PlayerController(const FObjectInitializer& ObjectInitializer)
{
#if PLATFORM_WINDOWS
	// We want the mouse cursor to show immediately on startup, 
	// without having to click in the window.
	bShowMouseCursor = true;
	bEnableTouchOverEvents = bEnableMouseOverEvents = true;

#elif PLATFORM_ANDROID || PLATFORM_IOS
	bShowMouseCursor = false;
#endif

	// We need click/touch events to interact with our tiles.
	bEnableTouchEvents = bEnableClickEvents = true;
	
	ScoreChangeRate = 375.0f;
}

void AMatch3PlayerController::AddScore(int32 Points, bool bForceImmediateUpdate)
{
	Score += Points;
	if (bForceImmediateUpdate)
		DisplayedScore = (float)Score;
	else
		GetWorldTimerManager().SetTimer(TickScoreDisplayHandle, this, &AMatch3PlayerController::TickScoreDisplay, 0.001f, false);
}

int32 AMatch3PlayerController::CalculateBombPower_Implementation()
{
	return 0;
}

void AMatch3PlayerController::TickScoreDisplay()
{
	DisplayedScore += UGameplayStatics::GetWorldDeltaSeconds(this) * ScoreChangeRate;
	if (DisplayedScore >= (float)Score)
	{
		DisplayedScore = Score;
		GetWorldTimerManager().ClearTimer(TickScoreDisplayHandle);
	}
}