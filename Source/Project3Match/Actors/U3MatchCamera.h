// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"

#include "../MISC/HelperClasses.h"

#include "U3MatchCamera.generated.h"

UCLASS(meta =(BlueprintSpawnableComponent) )
class PROJECT3MATCH_API AU3MatchCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:	
	AU3MatchCamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = U3MatchCameraHelper)
		void UpdateCamera();
	UFUNCTION(BlueprintCallable, Category = U3MatchCameraHelper)
		void CheckViewport();
	UFUNCTION(BlueprintCallable, Category = U3MatchCameraHelper)
		FVector MoveCameraZ();
	FORCEINLINE void SetCameraSet(bool bIsSet) { this->bIsCameraSet = bIsSet; }

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true), Category = U3MatchCameraVariable)
		class UGameInstance* GlobalGameInstance;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = U3MatchCameraVariable)
		class APaperSpriteActor* GridBorder;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true), Category = U3MatchCameraVariable)
		bool bIsCameraSet = false;
	
	//TUniquePtr<class FDoOnceObject> Once;
	HelperClasses::FDoOnceObject* DoOnce;
	FVector2D DefaultFit;
	FVector2D StoreSize;
	float DesiredSize;
};

//	초기화 방법
//  TSharedPtr<TestClass> ObjPtr = MakeShareable(new TestClass());

/*
*	초기화 및 형변환 방법

	TWeakObjectPtr<AActor> MyWeakActor;	
	MyWeakActor = MyActor; 
	AActor* Actor = MyWeakActor.Get(); 
	ACharacter* Character = Cast<ACharacter>(MyWeakActor);
	MyActor->Destroy();
	bool IsValid = MyWeakActor.Get() != nullptr;    //false
*/

/*
	APawn* TestPawn = GetWorld()->SpawnActor<APawn>(MyPawnClass, FVector(100.f, 100.f, 0.f), FRotator::ZeroRotator);
	APawn* MyPawn = GetWorld()->SpawnActor<APawn>(MyPawnClass, FVector(200.f, 200.f, 0.f), FRotator::ZeroRotator);

	TestArray.Add(TWeakObjectPtr<APawn>(TestPawn));
	TestArray.Add(TWeakObjectPtr<APawn>(MyPawn));

	int Num = TestArray.Num();  // 2

	TWeakObjectPtr<APawn> WeakPtr1(TestPawn);
	TestArray.Remove(WeakPtr1);
	int Num = TestArray.Num();  // 1

	TWeakObjectPtr<APawn> WeakPtr2(TestPawn);
	TestArray.Remove(WeakPtr2);
	int Num2 = TestArray.Num(); // 1
*/
