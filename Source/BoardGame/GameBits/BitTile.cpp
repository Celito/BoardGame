// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardGame.h"
#include "BitTile.h"


// Sets default values
ABitTile::ABitTile()
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

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh"));
	TileMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	TileMesh->OnClicked.AddDynamic(this, &ABitTile::TileClicked);
	TileMesh->AttachTo(DummyRoot);
}

// Called when the game starts or when spawned
void ABitTile::BeginPlay()
{
	Super::BeginPlay();	
}

void ABitTile::TileClicked(UPrimitiveComponent* ClickedComp)
{
	if (!Data.expired())
	{
		auto dataPtr = Data.lock();
		auto bitUniqueId = dataPtr->get_unique_id();
		FString bitName = dataPtr->get_bit_name().c_str();
		UE_LOG(LogTemp, Warning, TEXT("CLICKED ON TILE: %s, UNIQUE ID: %d"), *bitName, bitUniqueId)
	}
}
