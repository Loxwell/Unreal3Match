// Fill out your copyright notice in the Description page of Project Settings.


#include "Project3MatchGameModeBase.h"
#include "MISC/Match3BPFunctionLibrary.h"
#include "System/Match3PlayerController.h"

void AProject3MatchGameModeBase::GameOver()
{

}

int32 AProject3MatchGameModeBase::CalculateBombPower_Implementation()
{
	if (AMatch3PlayerController* PC = 
		Cast<AMatch3PlayerController>(UMatch3BPFunctionLibrary::GetLocalPlayerController(this)))
		return PC->CalculateBombPower();
	return 0;
}

int32 AProject3MatchGameModeBase::GetMaxComboPower()
{
	return 0;
}

int32 AProject3MatchGameModeBase::GetComboPower()
{
	return 0;
}

void AProject3MatchGameModeBase::SetComboPower(int32 NewComboPower)
{
}

void AProject3MatchGameModeBase::AddScore(int32 Points)
{

}