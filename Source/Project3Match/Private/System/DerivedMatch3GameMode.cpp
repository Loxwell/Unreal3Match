// Fill out your copyright notice in the Description page of Project Settings.
#include "System/DerivedMatch3GameMode.h"
#include "Actors/U3MatchCamera.h"
#include "MISC/Match3BPFunctionLibrary.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Math/UnrealMathUtility.h"

ADerivedMatch3GameMode::ADerivedMatch3GameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, ElapsedTime(0)
{
	ConstructorHelpers::FClassFinder<UUserWidget> UI(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_UIMainScreen.BP_UIMainScreen_C'"));
	if (UI.Succeeded())
		UIClass = UI.Class;
	check(UIClass);
}

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

	if (UIClass)
	{
		MainUI = CreateWidget(UMatch3BPFunctionLibrary::GetLocalPlayerController(this), UIClass, TEXT("SCREEN_UI"));
		MainUI->AddToViewport();
		UUserWidget* GameScreen = Cast<UUserWidget>(MainUI->WidgetTree->FindWidget(FName("BP_UIDisplay")));

		if (GameScreen)
		{
			UE_LOG(LogTemp, Warning, TEXT("Find BP_GameScreen"));
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Not found BP_GameScreen"));
		
		TextScore = Cast<UTextBlock>(GameScreen->WidgetTree->FindWidget(FName("Score")));
		/*Cast<UTextBlock>(GameScreen->WidgetTree->FindWidget(FName("Score")))->SetText(FText::AsNumber(10));
		FText Temp = Cast<UTextBlock>(GameScreen->WidgetTree->FindWidget(FName("Score")))->Text;
		FText GetText  = Cast<UTextBlock>(GameScreen->WidgetTree->FindWidget(FName("Score")))->GetText();
		UE_LOG(LogTemp, Warning, TEXT("%s, %s, %s"), *Temp.ToString(), *GetText.ToString(), *(FText::AsNumber(10).ToString()));*/

	}
}

void ADerivedMatch3GameMode::BeginDestroy()
{
	Super::BeginDestroy();
	UpdateTimeDisplay.Clear();
}

void ADerivedMatch3GameMode::Tick(float DeltaSeconds)
{
	ElapsedTime += DeltaSeconds;
	FNumberFormattingOptions Option;
	Option.SetAlwaysSign(true).SetUseGrouping(true).SetMinimumIntegralDigits(1).SetMinimumFractionalDigits(1).SetMaximumFractionalDigits(1);
	
	TextScore->SetText(FText::AsNumber(ElapsedTime, &Option));
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