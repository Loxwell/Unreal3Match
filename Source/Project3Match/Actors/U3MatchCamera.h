// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
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
	UFUNCTION(BlueprintCallable, Category = U3MatchCameraHelper)
		void UpdateCamera();
	UFUNCTION(BlueprintCallable, Category = U3MatchCameraHelper)
		void CheckViewport();
	UFUNCTION(BlueprintCallable, Category = U3MatchCameraHelper)
		FVector MoveCameraZ();

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true), Category = U3MatchCameraVariable)
		class UGameInstance* GameInstance;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = U3MatchCameraVariable)
		class APaperSpriteActor* GridBorder;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true), Category = U3MatchCameraVariable)
		bool bIsCameraSet = false;

	FVector2D DefaultFit = FVector2D(640, 1136);
	FVector2D StoreSize;
	float DesiredSize = 256;
};
