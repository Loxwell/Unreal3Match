
#include "Actors/Tile.h"
#include "Actors/Grid.h"
#include "../Project3MatchGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceConstant.h"

using AMatch3GameMode = AProject3MatchGameModeBase;

ATile::ATile()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (RootComponent)
		RootComponent->SetMobility(EComponentMobility::Movable);
	SetCanBeDamaged(false);
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

	Grid = Cast<AGrid>(GetOwner());

	AutoReceiveInput = EAutoReceiveInput::Player0;
	InputPriority = 0;

	/*AActor::*/OnInputTouchBegin.AddUniqueDynamic(this, &ATile::TilePress);
	/*AActor::*/OnInputTouchEnter.AddUniqueDynamic(this, &ATile::TileEnter);
}

void ATile::TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	if (!UGameplayStatics::IsGamePaused(this) && Grid)
		Grid->OnTileWasSelected(this);
}

void ATile::TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	// We have moved into the tile's space while we had a different tile selected. 
	// This is the same as pressing the tile directly.
	// Note that we need to make sure it's a different actual tile (i.e. not NULL) 
	// because deselecting a tile by touching it twice will then trigger the TileEnter event and re-select it.
	if (!UGameplayStatics::IsGamePaused(this) && Grid)
	{
		ATile* CurrentlySelectedTile = Grid->GetCurrentlySelectedTile();
		// ���� ���� �� Ÿ���� �ڽ��� �ƴ� ���
		if (CurrentlySelectedTile && (CurrentlySelectedTile != this))
			TilePress(FingerIndex, TouchedActor);
	}
}

void ATile::TilePress_Mouse(AActor* ClickedActor, FKey ButtonClicked)
{
	TilePress(ETouchIndex::Touch1, ClickedActor);
}

void ATile::TileEnter_Mouse(AActor* MouseOverActor)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if(PC && PC->IsInputKeyDown(EKeys::LeftMouseButton))
			TileEnter(ETouchIndex::Touch1, MouseOverActor);
	}
}

void ATile::StartFalling(bool bUseCurrentWorldLocation)
{
	float FallDistance = 0;

	FallingStartTime = GetWorld()->GetTimeSeconds();
	FallingStartLocation = GetActorLocation();

	// 120 FPS
	GetWorldTimerManager().SetTimer(TickFallingHandle, this, &ATile::TickFalling, 0.001f, true);
	check(Grid);

	if (!bUseCurrentWorldLocation)
	{
		// �Ʒ��� �̵� �� Grid Index�� Offset
		// X ��ǥ�� ���� ���� �� ��
		int32 YOffset = 0;
		
		// �ٴ����� ������ ����
		// HeightAboveBottom = 1�� ��ǥ ��ġ �Ʒ��� �ٸ� Ÿ���� ���� �� Grid ���� �̳� ����
		// HeightAboveBottom = 0�� ��ǥ ��ġ �Ʒ��� �ٸ� Ÿ�ϱ��� ���� �� Grid ���� �� ����
		int32 HeightAboveBottom = 1;

		while (true)
		{
			++YOffset;
			
			if (Grid->GetGridAddressWithOffset(GetGridAddress(), 0, -YOffset, LandingGridAddress)) 
			{
				if (ATile* TileBelow = Grid->GetTileFromGridAddress(LandingGridAddress))
				{
					switch (TileBelow->TileState)
					{
					case ETileState::ETS_FALLING:
						// This space contains a falling tile, so continue to fall through it, 
						// but note that the tile will land underneath us, so we need to leave a gap for it.
						// ���� Ÿ���� �Ʒ� Ÿ���� �������� ����
						// ���� �������� ������ �Ʒ��� Ÿ�ϰ� ������ �����ؾ���
						++HeightAboveBottom;
						continue;
					case ETileState::ETS_PENDING_DELETE:
						// This space contains a tile that is about to be deleted.
						// We can fall through this space freely.
						continue;
					}
				}
				else
					// The space below is empty, but is on the grid. 
					// We can fall through this space freely.
					continue;
			}
			// This space is off the grid or contains a tile that is staying. Go back one space and stop.
			// �Ʒ��� �̵� �� ĭ�� - �ٴ����� ���� �������� �� ĭ ��
			YOffset -= HeightAboveBottom;
			Grid->GetGridAddressWithOffset(GetGridAddress(), 0, -YOffset, LandingGridAddress);
			break;
		}

		FallDistance = Grid->TileSize.Y * YOffset;
		FallingEndLocation = FallingStartLocation;
		FallingEndLocation.Z -= FallDistance;
	}
	else
	{
		LandingGridAddress = GetGridAddress();
		FallingEndLocation = Grid->GetLocationFromGridAddress(LandingGridAddress);
		// ���� ó���� �Ǿ� ����
		// ���� ó���� �߰� �ؾ���
		FallDistance = FallingStartLocation.Z - FallingEndLocation.Z;
	}

	AMatch3GameMode* CurrentGameMode = Cast<AMatch3GameMode>(UGameplayStatics::GetGameMode(this));
	TotalFallingTime = 0;
	if (CurrentGameMode && (CurrentGameMode->GetTileMoveSpeed() > 0))
		TotalFallingTime = FallDistance / CurrentGameMode->GetTileMoveSpeed();
	if (TotalFallingTime <= 0)
		TotalFallingTime = .75f;
	StartFallingEffect();
}

void ATile::TickFalling()
{
	if (AMatch3GameMode* CurrentGameMoe = Cast<AMatch3GameMode>(UGameplayStatics::GetGameMode(this)))
	{
		check(Grid);
		check(TotalFallingTime > 0);
		// �̵� �ð� ����ȭ 
		float FallCompleteFraction = (GetWorld()->GetTimeSeconds() - FallingStartTime) / TotalFallingTime;

		// ����ȭ ���� 1�̻��̸� ����
		if (FallCompleteFraction >= 1)
			FinishFalling();
		else
			SetActorLocation(FMath::Lerp(FallingStartLocation, FallingEndLocation, FallCompleteFraction));
	}
	else
	{
		// ���� ����, ��ǥ ��ġ�� ���� �̵�
		FinishFalling();
	}
}

void ATile::FinishFalling()
{
	GetWorldTimerManager().ClearTimer(TickFallingHandle);
	SetActorLocation(FallingEndLocation);
	Grid->OnTileFinishedFalling(this, LandingGridAddress);
	StopFallingEffect();
}

void ATile::OnMatched_Implementation(ETileMoveType::Type MoveType)
{
	Grid->OnTileFinishedMatching(this);
}

void ATile::OnSwapMove_Implementation(ATile* OtherTile, bool bMoveWillSuccess)
{
	Grid->OnSwapDisplayFinished(this);
}

void ATile::SetTileMaterial_Implementation(UMaterialInstanceConstant* TileMaterial)
{
	// ��� ����Ʈ ������ c++�� ��ȯ �� ��
	GetRenderComponent()->SetMaterial(0, TileMaterial);
}

USoundWave* ATile::GetMatchSound()
{
	return MatchSound;
}