// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/DerivedTest.h"

void ADerivedTest::NativeEvent_Implementation(int32 Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ADerivedTest::NativeEvent_Implementation(%d)"), Value);
	FTimerHandle Handler;
	FTimerDelegate Delegate;
	Delegate.BindLambda([this](int32 Param) {
		Super::NativeEvent_Implementation(Param + 1);
	}, Value);
	
	GetWorld()->GetTimerManager().SetTimer(Handler, Delegate, 1.f, false);
}