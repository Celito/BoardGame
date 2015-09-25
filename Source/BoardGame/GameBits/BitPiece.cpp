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

	PieceHighlightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightMesh"));
	PieceHighlightMeshComponent->AttachTo(PieceMeshComponent);

	//UPostProcessComponent* postProcessComp;
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
	PieceMeshComponent->OnEndCursorOver.AddDynamic(this, &ABitPiece::PieceMouseOut);

	FVector orgin;
	FVector boundsExtent;
	GetActorBounds(false, orgin, boundsExtent);

	// Update Width whenever is added a mesh to the piece;
	Width = boundsExtent.X * 2;

	//Setting the highlight effect
	PieceHighlightMeshComponent->SetStaticMesh(PieceMesh);

	auto world = GetWorld();
	auto gameMode = (ABoardGameGameMode*)world->GetAuthGameMode();
	PieceHighlightMeshComponent->SetRenderCustomDepth(true);
	PieceHighlightMeshComponent->SetMaterial(0, gameMode->HighlightMaterial);
	PieceHighlightMeshComponent->SetVisibility(false); 
	
}



const float ABitPiece::GetWidth() const
{
	return Width;
}

void ABitPiece::SetData(const shared_ptr<GameBit> &data)
{
	Super::SetData(data);

	FString bitName = data->get_bit_name().c_str();
	uint32 collor = data->get_attr("Color").get_value();

	auto world = GetWorld();
	auto gameMode = (ABoardGameGameMode*)world->GetAuthGameMode();

	FString meshKey = bitName + (collor == 0 ? TEXT("White") : TEXT("Black"));
	UStaticMesh* mesh = gameMode->LoadedMeshes[meshKey];
	uint32 materialsNum = mesh->Materials.Num();
	if (materialsNum > 1)
	{
		mesh->Materials.RemoveAt(1, materialsNum - 2);
	}
	AddPieceMesh(mesh);
}

void ABitPiece::PieceClicked(UPrimitiveComponent* ClickedComp)
{
	auto gameMode = GetWorld()->GetAuthGameMode();
	auto boardGameMode = dynamic_cast<ABoardGameGameMode*>(gameMode);
	
	if (boardGameMode != nullptr && !Data.expired() && IsAnOption)
	{
		auto dataPtr = Data.lock();
		auto bitUniqueId = dataPtr->get_unique_id();
		FString bitName = dataPtr->get_bit_name().c_str();
		UE_LOG(LogTemp, Warning, TEXT("SELECTED PIECE: %s, UNIQUE ID: %d"), *bitName, bitUniqueId);
		boardGameMode->ChooseOptionByBit(bitUniqueId);
	}
}

void ABitPiece::PieceMouseOver(UPrimitiveComponent* ClickedComp)
{

}

void ABitPiece::PieceMouseOut(UPrimitiveComponent* ClickedComp)
{

}

void ABitPiece::SetAsAnOption(bool isAnOption)
{
	Super::SetAsAnOption(isAnOption);
	PieceHighlightMeshComponent->SetVisibility(isAnOption);
}