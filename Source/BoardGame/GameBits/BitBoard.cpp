// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardGame.h"
#include <gameBits/GameBit.h>
#include <gameBits/boards/Tile.h>
#include "BitBoard.h"
#include "BitTile.h"


// Sets default values
ABitBoard::ABitBoard()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Meshes/DummyMesh"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	USceneComponent* DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	/*BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh"));
	BoardMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BoardMesh->AttachTo(DummyRoot);*/

}

// Called when the game starts or when spawned
void ABitBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABitBoard::SetData(const shared_ptr<Board> &data)
{
	auto children = data->get_children();
	for (auto child : children)
	{
		auto childPtr = child.lock();
		if (auto tilePtr = dynamic_pointer_cast<Tile>(childPtr))
		{
			auto visualTile = GetWorld()->SpawnActor<ABitTile>(ABitTile::StaticClass());

			visualTile->AttachRootComponentToActor(this);
			visualTile->SetData(tilePtr);
		}
	}
}