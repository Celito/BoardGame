// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BoardGame.h"
#include "BoardGameGameMode.h"
#include "BoardGamePlayerController.h"
#include "BoardGameCharacter.h"
#include "BitsManager.h"
#include "GameBits/BitPieceSet.h"
#include "GameBits/BitPiece.h"
#include "GameBits/BitBoard.h"
#include <turns/actions/options/Option.h>
#include <turns/actions/options/BitOption.h>
#include "gameBits/GameBit.h"
#include "gameBits/PieceSet.h"
#include "gameBits/VisualGameBit.h"
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

	FTimerHandle MyHandle;

	CurrOptionsSetuped = false;
	
	GetWorldTimerManager().SetTimer(MyHandle, this, &ABoardGameGameMode::CheckBgCoreStatus, 1, true);
}

void ABoardGameGameMode::CheckBgCoreStatus()
{
	if (!CurrOptionsSetuped && CoreThread && CoreThread->IsWaitingForChoise())
	{
		auto opts = CoreThread->GetCurrOptions();
		UE_LOG(LogTemp, Warning, TEXT("CORE IS DONE AND THERE ARE %d OPTIONS"), opts.size());
		for (auto opt : opts)
		{
			if (auto bitOpt = dynamic_pointer_cast<BitOption>(opt))
			{
				auto bit = bitOpt->get_bit();
				auto uniqueId = bit->get_unique_id();
				if (RegisteredBits.find(uniqueId) != RegisteredBits.end())
				{
					RegisteredBits[uniqueId]->SetAsAnOption(true);
				}
			}
		}
		CurrOptionsSetuped = true;
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

void ABoardGameGameMode::RegisterVisualBit(AVisualGameBit* bit)
{
	RegisteredBits[bit->GetUniqueDataId()] = bit;
}

void ABoardGameGameMode::ChooseOptionByBit(uint32 option_id)
{
	auto opts = CoreThread->GetCurrOptions();
	for (auto opt : opts)
	{
		if (auto optBit = dynamic_pointer_cast<BitOption>(opt))
		{
			if (option_id == optBit->get_bit()->get_unique_id())
			{
				ChooseOption(opt);
			}
		}
	}
}

void ABoardGameGameMode::ChooseOption(shared_ptr<Option> opt)
{
	auto opts = CoreThread->GetCurrOptions();
	for (auto opt : opts)
	{
		if (auto optBit = dynamic_pointer_cast<BitOption>(opt))
		{
			auto uniqueId = optBit->get_bit()->get_unique_id();
			if (RegisteredBits.find(uniqueId) != RegisteredBits.end())
			{
				RegisteredBits[uniqueId]->SetAsAnOption(false);
			}
		}
	}
	CoreThread->Choose(opt);
	CurrOptionsSetuped = false;
}