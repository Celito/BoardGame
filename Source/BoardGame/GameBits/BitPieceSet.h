// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VisualGameBit.h"
#include "gameBits/PieceSet.h"
#include "BitPieceSet.generated.h"

UCLASS()
class BOARDGAME_API ABitPieceSet : public AVisualGameBit
{
	GENERATED_BODY()

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;
	
public:	
	// Sets default values for this actor's properties
	ABitPieceSet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetData(const shared_ptr<GameBit> &data) override;
};
