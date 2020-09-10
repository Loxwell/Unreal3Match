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
	/// ����ڰ� Ÿ���� ���� ������ ���� �Լ�
	/// </summary>
	UFUNCTION()
		void TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor);
	/// <summary>
	/// Ÿ���� ���� �� �̸����� ������ �� ȣ�� �Լ�
	/// </summary>
	UFUNCTION()
		void TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor);
	/// <summary>
	/// ���콺�� Ÿ�� ����
	/// </summary>
	UFUNCTION()
		void TilePress_Mouse(AActor* ClickedActor, FKey ButtonClicked);
	/// <summary>
	/// ������ Ÿ���� ���콺�� �̸����� ������ �� ȣ�� �Լ�
	/// </summary>
	UFUNCTION()
		void TileEnter_Mouse(AActor* MouseOverActor);
	/// <summary>
	/// ���� ��ġ�� ��ǥ ��ġ�� ������ ���� �̵�
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

	// �̺�Ʈ �޽��� ��� ������ ������ ���� ã�� �� ��
	UFUNCTION(BlueprintImplementableEvent, Category = SPECIAL_TYPE)
		void StartFallingEffect();
	// �̺�Ʈ �޽��� ��� ������ ������ ���� ã�� �� ��
	UFUNCTION(BlueprintImplementableEvent, Category = SPECIAL_TYPE)
		void StopFallingEffect();
	// �̺�Ʈ �޽��� ��� ������ ������ ���� ã�� �� ��
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
	/// Ÿ���� ���� �� Grid Frame, ���� ���� �� BeginPlay()���� Outer ��ü�� ������ AGrid ����ȯ �� �Ҵ�
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = Tile)
		class AGrid* Grid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, Meta = (BlueprintProtected = true))
		USoundWave* MatchSound;

	FTimerHandle TickFallingHandle;
	/// <summary>
	/// �̵� ���� ����
	/// </summary>
	FVector FallingStartLocation;
	/// <summary>
	/// �̵� ��ǥ ����
	/// </summary>
	FVector FallingEndLocation;
	/// <summary>
	/// 1D Key/Value�ν� Grid�� Ÿ���� ������ Gird Address, ������� �ٴ����� ������ �� ���
	/// </summary>
	int32 LandingGridAddress;

	float TotalFallingTime;
	float FallingStartTime;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 TileTypeID;
	/// <summary>
	/// 1D Key/Value, �̿� Ÿ�� �˻� �� �� ���
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 GridAddress;
	UPROPERTY()
		TEnumAsByte<ETileState::Type> TileState;
};