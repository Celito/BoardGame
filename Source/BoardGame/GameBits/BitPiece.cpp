// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardGame.h"
#include "BitPiece.h"
#include "../BoardGameGameMode.h"


// Sets default values
ABitPiece::ABitPiece()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// TODO: Add a block in game to know the position and the limits of the pieces set;
	PieceMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	PieceMeshComponent->AttachTo(DummyRoot);
}

// Called when the game starts or when spawned
void ABitPiece::BeginPlay()
{
	Super::BeginPlay();	
}

void ABitPiece::AddPieceMesh(UStaticMesh *PieceMesh)
{
	PieceMeshComponent->SetStaticMesh(PieceMesh);

	PieceMeshComponent->OnClicked.AddDynamic(this, &ABitPiece::PieceClicked);
	PieceMeshComponent->OnBeginCursorOver.AddDynamic(this, &ABitPiece::PieceMouseOver);

	FVector orgin;
	FVector boundsExtent;
	GetActorBounds(false, orgin, boundsExtent);

	// Update Width whenever is added a mesh to the piece;
	Width = boundsExtent.X * 2;
}

const float ABitPiece::GetWidth() const
{
	return Width;
}

void ABitPiece::SetData(shared_ptr<Piece> data)
{
	Data = data;
	FString bitName = data->get_bit_name().c_str();
	uint32 collor = data->get_attr("Color").get_value();

	auto world = GetWorld();
	auto gameMode = (ABoardGameGameMode*)world->GetAuthGameMode();

	FString meshKey = bitName + (collor == 0 ? TEXT("White") : TEXT("Black"));
	AddPieceMesh(gameMode->LoadedMeshes[meshKey]);
}

void ABitPiece::PieceClicked(UPrimitiveComponent* ClickedComp)
{
	if (!Data.expired())
	{
		auto dataPtr = Data.lock();
		auto bitUniqueId = dataPtr->get_unique_id();
		FString bitName = dataPtr->get_bit_name().c_str();
		UE_LOG(LogTemp, Warning, TEXT("CLICKED ON PIECE: %s, UNIQUE ID: %d"), *bitName, bitUniqueId)
	}
}

void ABitPiece::PieceMouseOver(UPrimitiveComponent* ClickedComp)
{

}