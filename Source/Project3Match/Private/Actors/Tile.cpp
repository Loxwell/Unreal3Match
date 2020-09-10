
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
		// 현재 선택 된 타일이 자신이 아닌 경우
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
		// 아래로 이동 할 Grid Index의 Offset
		// X 좌표도 추후 수정 할 것
		int32 YOffset = 0;
		
		// 바닥으로 부터의 높이
		// HeightAboveBottom = 1은 목표 위치 아래에 다른 타일이 존재 및 Grid 범위 이내 영역
		// HeightAboveBottom = 0은 목표 위치 아래에 다른 타일까지 포함 및 Grid 범위 외 영역
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
						// 현재 타일의 아래 타일은 떨어지는 상태
						// 같이 떨어지기 때문에 아래의 타일과 간격을 유지해야함
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
			// 아래로 이동 할 칸수 - 바닥으로 부터 떨어져야 할 칸 수
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
		// 수직 처리만 되어 있음
		// 수평 처리도 추가 해야함
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
		// 이동 시간 정규화 
		float FallCompleteFraction = (GetWorld()->GetTimeSeconds() - FallingStartTime) / TotalFallingTime;

		// 정규화 비율 1이상이면 종료
		if (FallCompleteFraction >= 1)
			FinishFalling();
		else
			SetActorLocation(FMath::Lerp(FallingStartLocation, FallingEndLocation, FallCompleteFraction));
	}
	else
	{
		// 오류 상태, 목표 위치로 강제 이동
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
	// 블루 프린트 내용을 c++로 변환 할 것
	GetRenderComponent()->SetMaterial(0, TileMaterial);
}

USoundWave* ATile::GetMatchSound()
{
	return MatchSound;
}