// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VisualGameBit.h"
#include <memory>
#include <gameBits/boards/Tile.h>
#include "BitTile.generated.h"

UCLASS()
class BOARDGAME_API ABitTile : public AVisualGameBit
{
	GENERATED_BODY()

	UStaticMeshComponent *TileMesh;
	
	UFUNCTION()
	void TileClicked(UPrimitiveComponent* ClickedComp);

public:	
	// Sets default values for this actor's properties
	ABitTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
