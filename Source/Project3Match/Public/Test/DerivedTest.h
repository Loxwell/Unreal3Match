// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/Test.h"
#include "DerivedTest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT3MATCH_API ADerivedTest : public ATest
{
	GENERATED_BODY()
	
public:
	virtual void NativeEvent_Implementation(int32 Value = -2) override;
};
