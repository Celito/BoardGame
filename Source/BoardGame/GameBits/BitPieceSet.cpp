// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardGame.h"
#include "BitPieceSet.h"
#include "BitPiece.h"
#include "../BoardGameGameMode.h"


// Sets default values
ABitPieceSet::ABitPieceSet()
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

	// TODO: Add a block in game to know the position and the limits of the pieces set;
	//BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	//BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	//BlockMesh->AttachTo(DummyRoot);

}

// Called when the game starts or when spawned
void ABitPieceSet::BeginPlay()
{
	Super::BeginPlay();	
}

void ABitPieceSet::SetData(const shared_ptr<GameBit> &data)
{
	Super::SetData(data);

	float betweenPieces = 5.f;
	float totalPiecesWidth = 0;
	uint16 pieceCount = 0;

	vector<ABitPiece*> visualPieces;

	//TODO: Load the initial state of the piece set
	for (auto childData : data->get_children())
	{
		if (auto pieceData = dynamic_pointer_cast<Piece>(childData.lock()))
		{

			auto pieceVisual = GetWorld()->SpawnActor<ABitPiece>(ABitPiece::StaticClass());

			pieceVisual->AttachRootComponentToActor(this);
			pieceVisual->SetData(pieceData);

			visualPieces.push_back(pieceVisual);

			totalPiecesWidth += pieceVisual->GetWidth();

			/*UE_LOG(LogTemp, Warning, TEXT("LOADING PIECE: %s, ORIGIN: %s, BOUNDS EXTENT: %s"), 
				*bitName, *orgin.ToCompactString(), *boundsExtent.ToCompactString())*/
		}
	}
	// Positioned all the pieces:
	FVector currPosition(0.f, 0.f, 0.f);
	currPosition.X = -((totalPiecesWidth + ((visualPieces.size() - 1) * betweenPieces)) / 2.f);
	for (auto visualPiece : visualPieces)
	{
		currPosition.X += visualPiece->GetWidth() / 2.f;
		visualPiece->SetActorRelativeLocation(currPosition);
		currPosition.X += (visualPiece->GetWidth() / 2.f) + betweenPieces;
	}
}

