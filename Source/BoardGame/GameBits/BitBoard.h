// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <memory>
#include "gameBits/boards/Board.h"
#include "BitBoard.generated.h"

UCLASS()
class BOARDGAME_API ABitBoard : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent *BoardMesh;
	
public:	
	// Sets default values for this actor's properties
	ABitBoard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SetData(const shared_ptr<Board> &data);	
};
