// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VisualGameBit.h"
#include <memory>
#include "gameBits/boards/Board.h"
#include "BitBoard.generated.h"

UCLASS()
class BOARDGAME_API ABitBoard : public AVisualGameBit
{
	GENERATED_BODY()

	UStaticMeshComponent *BoardMesh;
	
public:	
	// Sets default values for this actor's properties
	ABitBoard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void SetData(const shared_ptr<GameBit> &data) override;	
};
