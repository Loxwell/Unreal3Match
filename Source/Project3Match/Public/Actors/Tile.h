#pragma once
#include "PaperSpriteActor.h"
#include "PaperSpriteComponent.h"

#include "../MISC/TileStructure.h"
#include "../MISC/EnumLibrary.h"

#include "Tile.generated.h"

#define SPECIAL_TYPE "Special Game Events"

UCLASS()
class PROJECT3MATCH_API ATile : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	ATile();

	virtual void BeginPlay() override;

	/// <summary>
	/// 사용자가 타일을 선택 시작한 시점 함수
	/// </summary>
	UFUNCTION()
		void TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor);
	/// <summary>
	/// 타일을 접촉 후 이리저리 움직일 때 호출 함수
	/// </summary>
	UFUNCTION()
		void TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor);
	/// <summary>
	/// 마우스로 타일 선택
	/// </summary>
	UFUNCTION()
		void TilePress_Mouse(AActor* ClickedActor, FKey ButtonClicked);
	/// <summary>
	/// 선택한 타일을 마우스로 이리저리 움직일 때 호출 함수
	/// </summary>
	UFUNCTION()
		void TileEnter_Mouse(AActor* MouseOverActor);
	/// <summary>
	/// 현재 위치와 목표 위치를 보간을 통해 이동
	/// </summary>
	UFUNCTION()
		void TickFalling();

#pragma region BLUEPRINT_EVENT
	UFUNCTION(BlueprintNativeEvent, Category = SPECIAL_TYPE)
		void SetTileMaterial(class UMaterialInstanceConstant* TileMaterial);
	virtual void SetTileMaterial_Implementation(class UMaterialInstanceConstant* TileMaterial);

	UFUNCTION(BlueprintNativeEvent, Category = SPECIAL_TYPE)
		void OnSwapMove(ATile * OtherTile, bool bMoveWillSuccess);
	virtual void OnSwapMove_Implementation(ATile* OtherTile, bool bMoveWillSuccess);
	
	UFUNCTION(BlueprintNativeEvent)
		UObject* GetBPReference();
	virtual UObject* GetBPReference_Implementation() { return nullptr; };

	/// <summary>
	/// 
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = SPECIAL_TYPE)
		void OnMatched(ETileMoveType::Type MoveType);
	virtual void OnMatched_Implementation(ETileMoveType::Type MoveType);

	// 이벤트 메신저 기법 구현을 디자인 패턴 찾아 볼 것
	UFUNCTION(BlueprintImplementableEvent, Category = SPECIAL_TYPE)
		void StartFallingEffect();
	// 이벤트 메신저 기법 구현을 디자인 패턴 찾아 볼 것
	UFUNCTION(BlueprintImplementableEvent, Category = SPECIAL_TYPE)
		void StopFallingEffect();
	// 이벤트 메신저 기법 구현을 디자인 패턴 찾아 볼 것
	UFUNCTION(BlueprintImplementableEvent, Category = SPECIAL_TYPE)
		void PlaySelectionEffect(bool bTurnEffectOn);
#pragma endregion

#pragma region NORMAL_CPP_FUCTIONS
	void StartFalling(bool bUseCurrentWorldLocation = false);
	void FinishFalling();
	USoundWave* GetMatchSound();
#pragma endregion

#pragma region Get/Set
public:
	inline int32 GetGridAddress() const { return GridAddress; }
	inline void SetGridAddress(int32 NewGridAddress) { GridAddress = NewGridAddress; }

	inline int32 GetTileTypeID()const { return TileTypeID; }
	inline void SetTileTypeID(const int32& NewTypeID) { TileTypeID = NewTypeID; }

	inline void SetTileState(const TEnumAsByte<ETileState::Type>& NewState) { TileState = NewState; }
#pragma endregion

public:
	UPROPERTY(BlueprintReadOnly)
		FTileAbilities Abilities;

protected:
	/// <summary>
	/// 타일을 소유 한 Grid Frame, 동적 생성 시 BeginPlay()에서 Outer 객체의 참조를 AGrid 형변환 후 할당
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Tile)
		class AGrid* Grid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, Meta = (BlueprintProtected = true))
		USoundWave* MatchSound;

	FTimerHandle TickFallingHandle;
	/// <summary>
	/// 이동 시작 지점
	/// </summary>
	FVector FallingStartLocation;
	/// <summary>
	/// 이동 목표 지점
	/// </summary>
	FVector FallingEndLocation;
	/// <summary>
	/// 1D Key/Value로써 Grid상에 타일이 떨어질 Gird Address, 허공에서 바닥으로 떨어질 때 사용
	/// </summary>
	int32 LandingGridAddress;

	float TotalFallingTime;
	float FallingStartTime;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 TileTypeID;
	/// <summary>
	/// 1D Key/Value, 이웃 타일 검색 할 때 사용
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 GridAddress;
	UPROPERTY()
		TEnumAsByte<ETileState::Type> TileState;
};