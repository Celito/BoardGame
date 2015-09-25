// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardGame.h"
#include "VisualGameBit.h"
#include "../BoardGameGameMode.h"


// Sets default values
AVisualGameBit::AVisualGameBit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVisualGameBit::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVisualGameBit::SetData(const shared_ptr<GameBit> &data)
{
	Data = data;
	AGameMode *gameMode = GetWorld()->GetAuthGameMode();
	ABoardGameGameMode *boardGM = dynamic_cast<ABoardGameGameMode *>(gameMode);
	boardGM->RegisterVisualBit(this);
}

uint32 AVisualGameBit::GetUniqueDataId()
{
	return Data.lock()->get_unique_id();
}