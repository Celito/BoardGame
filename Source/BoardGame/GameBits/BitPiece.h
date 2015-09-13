// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "gameBits/Piece.h"
#include "BitPiece.generated.h"

UCLASS()
class BOARDGAME_API ABitPiece : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PieceMeshComponent;

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PieceHighlightMeshComponent;

	float Width;

	weak_ptr<Piece> Data;

	/** Handle the piece being clicked */
	UFUNCTION()
	void PieceClicked(UPrimitiveComponent* ClickedComp);

	/** Handle the piece mouse over */
	UFUNCTION()
		void PieceMouseOver(UPrimitiveComponent* ClickedComp);

	/** Handle the piece mouse out */
	UFUNCTION()
		void PieceMouseOut(UPrimitiveComponent* ClickedComp);
	
public:	
	// Sets default values for this actor's properties
	ABitPiece();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AddPieceMesh(UStaticMesh *PieceMesh);

	const float GetWidth() const;

	void SetData(shared_ptr<Piece> data);
};
