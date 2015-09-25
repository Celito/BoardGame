// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <memory>
#include <gameBits/GameBit.h>
#include "VisualGameBit.generated.h"

UCLASS()
class BOARDGAME_API AVisualGameBit : public AActor
{
	GENERATED_BODY()
	
protected:
	weak_ptr<GameBit> Data;

	bool IsAnOption = false;

public:	
	// Sets default values for this actor's properties
	AVisualGameBit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetData(const shared_ptr<GameBit> &data);

	virtual void SetAsAnOption(bool isAnOption) { IsAnOption = isAnOption;  }
	
	uint32 GetUniqueDataId();
};
