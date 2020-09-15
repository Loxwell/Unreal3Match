// Fill out your copyright notice in the Description page of Project Settings.
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Math/UnrealMathUtility.h"

#include "Actors/U3MatchCamera.h"
#include "System/DerivedMatch3GameMode.h"
#include "MISC/Match3BPFunctionLibrary.h"

void ADerivedMatch3GameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> Array;
	UGameplayStatics::GetAllActorsOfClass(this, AU3MatchCamera::StaticClass(), Array);

	for (AActor* const Actor : Array)
	{
		AU3MatchCamera* const Camera = Cast<AU3MatchCamera>(Actor);
		if (Camera)
			Camera->UpdateCamera();
	}
	
	UpdateTimeDisplay.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(GameTimeHandler, this, &ADerivedMatch3GameMode::GameTimer, 1.0f, true);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI(TEXT("WidgetBlueprint'/Game/Blueprints/Test/BP_UITest.BP_UITest_C'"));

	if (UI.Succeeded())
		UIClass = UI.Class;
	
	check(UIClass);
}

void ADerivedMatch3GameMode::BeginDestroy()
{
	Super::BeginDestroy();
	UpdateTimeDisplay.Clear();

	if (UIClass)
	{
		MainUI = CreateWidget(UMatch3BPFunctionLibrary::GetLocalPlayerController(this), UIClass, TEXT("SCREEN_UI"));
		MainUI->AddToViewport();
		UUserWidget* GameScreen = Cast<UUserWidget>( MainUI->WidgetTree->FindWidget(FName("BP_GameScreen")) );

		if (GameScreen)
		{
			UE_LOG(LogTemp, Warning, TEXT("Find BP_GameScreen"));
		}else
			UE_LOG(LogTemp, Warning, TEXT("Not found BP_GameScreen"));
	}
}

void ADerivedMatch3GameMode::GameTimer()
{
	float Time = FCString::Atof(*Super::GetRemainingTimeAsString());
	TimeAsText = FText::AsNumber(Time);
	UpdateTimeDisplay.Broadcast();

	if (!Super::GetTimerPaused())
	{
		int iTime = FMath::TruncToInt(Time);
		//UE_LOG(LogTemp, Warning, TEXT("%d"), iTime);
		switch (iTime)
		{
		case 0:
			UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, GameTimeHandler);
			break;
		default:
			break;
		}
	}

	if (DoOnce)
		GameStarted.Broadcast();
}

bool ADerivedMatch3GameMode::TimeAddedCheck()
{
	return (FCString::Atof(*Super::GetRemainingTimeAsString()) > 10);
}

void ADerivedMatch3GameMode::SpawnScoreDisplay(FVector WorldLocaton)
{

}