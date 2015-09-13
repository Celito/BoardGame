// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BoardGame.h"
#include "BoardGameGameMode.h"
#include "BoardGamePlayerController.h"
#include "BoardGameCharacter.h"
#include "BitsManager.h"
#include "GameBits/BitPieceSet.h"
#include "GameBits/BitPiece.h"
#include "GameBits/BitBoard.h"
#include "gameBits/GameBit.h"
#include "gameBits/PieceSet.h"
#include "gameBits/boards/Board.h"

ABoardGameGameMode::ABoardGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABoardGamePlayerController::StaticClass();
	DefaultPawnClass = ABoardGameCharacter::StaticClass();

	// TODO: Simplify the meshes by using only 1 mesh, 1 material and different textures;
	// TEMP: Getting the used meshs, materials and textures here
	LoadedMeshes[TEXT("QueenBlack")] = 
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/queen_black_piece")).Get();
	LoadedMeshes[TEXT("GrasshopperBlack")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/grasshopper_black_piece")).Get();
	LoadedMeshes[TEXT("BeetleBlack")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/beetle_black_piece")).Get();
	LoadedMeshes[TEXT("SpiderBlack")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/spider_black_piece")).Get();
	LoadedMeshes[TEXT("AntBlack")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/ant_black_piece")).Get();
	LoadedMeshes[TEXT("QueenWhite")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/queen_white_piece")).Get();
	LoadedMeshes[TEXT("GrasshopperWhite")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/grasshopper_white_piece")).Get();
	LoadedMeshes[TEXT("BeetleWhite")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/beetle_white_piece")).Get();
	LoadedMeshes[TEXT("SpiderWhite")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/spider_white_piece")).Get();
	LoadedMeshes[TEXT("AntWhite")] =
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Meshes/ant_white_piece")).Get();
	HighlightMaterial =
		ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Materials/M_Highlight")).Get();

	// start the bgcore

	// TEMP: this configuration should be on a config file

	PlayerDistanceFromCenter = 250.0;
	TableHeight = 120.0;
}

void ABoardGameGameMode::BeginPlay()
{
	// TODO: load the choose board game


	// Load bits at the table
	auto tableBitsData = core.bits_manager()->get_table_bits();

	for (auto bitData : tableBitsData)
	{
		LoadBit(bitData);
	}

	// TODO: Start the game (differnt thread?)

	CoreThread = FBgCoreThread::JoyInit(*this);
	
	GetWorldTimerManager().SetTimer(this, &ABoardGameGameMode::CheckBgCoreStatus, 1, true);
}

void ABoardGameGameMode::CheckBgCoreStatus()
{
	if (CoreThread && CoreThread->IsWaitingForChoise())
	{
		UE_LOG(LogTemp, Warning, TEXT("CORE IS DONE AND THERE ARE %d OPTIONS"), CoreThread->GetNumOfOptions());
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("CORE IS NOT READY!"));
	}
}

void ABoardGameGameMode::LoadPlayer(const shared_ptr<Player> &player)
{
	auto children = player->get_children();

	float angleBetweenPlayers = 360.f / core.get_num_of_players();

	FVector initialPoint(0.f, PlayerDistanceFromCenter, TableHeight);
	uint32 player_id = player->get_id();
	float player_angle = angleBetweenPlayers * player_id;
	initialPoint = initialPoint.RotateAngleAxis(player_angle, FVector(0.f, 0.f, 1.f));

	for (auto child : children)
	{
		if (auto pieceSetData = dynamic_pointer_cast<PieceSet>(child.lock()))
		{
			UWorld* const World = GetWorld();
			auto pieceSetVisual = World->SpawnActor<ABitPieceSet>(ABitPieceSet::StaticClass());

			// TODO: set those from a game config
			pieceSetVisual->SetActorRelativeRotation(FRotator(0.f, 180.f - player_angle, 0.f));
			pieceSetVisual->SetActorRelativeLocation(initialPoint);

			pieceSetVisual->SetData(pieceSetData);
		}
	}
}

void ABoardGameGameMode::LoadBit(const shared_ptr<GameBit> &bitData)
{
	auto bitUniqueId = bitData->get_unique_id();
	FString bitName = bitData->get_bit_name().c_str();
	UE_LOG(LogTemp, Warning, TEXT("LOADING BIT: %s, UNIQUE ID: %d"), *bitName, bitUniqueId);

	if (auto playerDataPtr = dynamic_pointer_cast<Player>(bitData))
	{
		// Loading a player data
		LoadPlayer(playerDataPtr);
	}
	else if (auto boardDataPtr = dynamic_pointer_cast<Board>(bitData))
	{
		// Loading a board data
		UWorld* const World = GetWorld();
		auto boardVisual = World->SpawnActor<ABitBoard>(ABitBoard::StaticClass());

		FVector initialPoint(0.f, 0.f, TableHeight);

		boardVisual->SetActorRelativeLocation(initialPoint);

		boardVisual->SetData(boardDataPtr);
	}
}