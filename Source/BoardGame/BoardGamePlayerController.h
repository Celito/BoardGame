// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "BoardGamePlayerController.generated.h"

UCLASS()
class ABoardGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABoardGamePlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	bool rotatingLeft = false;
	bool rotatingRight = false;

	float RotationVelocity;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	void CameraZoomIn();

	void CameraZoomOut();

	void StartRotateLeft();

	void StopRotateLeft();

	void StartRotateRight();

	void StopRotateRight();

	float CameraZoomIncreese;

	float CameraZoomInLimit;

	float CameraZoomOutLimit;
};


