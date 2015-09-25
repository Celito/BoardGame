// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include <GameFramework/GameMode.h>
#include <BgCore.h>
#include <player/Player.h>
#include "GameBits/VisualGameBit.h"
#include "BgCoreThread/BgCoreThread.h"
#include "BoardGameGameMode.generated.h"

UCLASS(minimalapi)
class ABoardGameGameMode : public AGameMode
{
	GENERATED_BODY()

	double PlayerDistanceFromCenter;
	double TableHeight;

	void LoadPlayer(const shared_ptr<Player> &player);

	void LoadBit(const shared_ptr<GameBit> &bit);

	FBgCoreThread* CoreThread;

	void CheckBgCoreStatus();

	map<uint32, AVisualGameBit*> RegisteredBits;

	bool CurrOptionsSetuped;

public:
	ABoardGameGameMode();

	virtual void BeginPlay();

	map<FString, UStaticMesh *> LoadedMeshes;

	UMaterial* HighlightMaterial;

	BgCore core;

	void RegisterVisualBit(AVisualGameBit* bit);

	void ChooseOptionByBit(uint32 option_id);

	void ChooseOption(shared_ptr<Option> opt);
};



