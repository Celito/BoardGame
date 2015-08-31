// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BoardGame.h"
#include "BoardGamePlayerController.h"
#include "BoardGameCharacter.h"
#include "AI/Navigation/NavigationSystem.h"
#include "BGCore.h"
#include "BitsManager.h"

ABoardGamePlayerController::ABoardGamePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	CameraZoomIncreese = 25.0;
	CameraZoomInLimit = 200.0;
	CameraZoomOutLimit = 600.0;

	RotationVelocity = 60.f;
}

void ABoardGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if ((rotatingRight && !rotatingLeft) || (rotatingLeft && !rotatingRight))
	{
		auto character = (ABoardGameCharacter*)GetPawn();
		auto rotation = character->GetActorRotation();
		float deltaRotation = DeltaTime * RotationVelocity;
		rotation.Yaw += rotatingRight? deltaRotation : -deltaRotation;
		character->SetActorRotation(rotation);
	}
}

void ABoardGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ABoardGamePlayerController::CameraZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ABoardGamePlayerController::CameraZoomOut);

	InputComponent->BindAction("RotateLeft", IE_Pressed, this, &ABoardGamePlayerController::StartRotateLeft);
	InputComponent->BindAction("RotateLeft", IE_Released, this, &ABoardGamePlayerController::StopRotateLeft);

	InputComponent->BindAction("RotateRight", IE_Pressed, this, &ABoardGamePlayerController::StartRotateRight);
	InputComponent->BindAction("RotateRight", IE_Released, this, &ABoardGamePlayerController::StopRotateRight);
}

void ABoardGamePlayerController::StartRotateLeft()
{
	rotatingLeft = true;
}

void ABoardGamePlayerController::StopRotateLeft()
{
	rotatingLeft = false;
}

void ABoardGamePlayerController::StartRotateRight()
{
	rotatingRight = true;
}

void ABoardGamePlayerController::StopRotateRight()
{
	rotatingRight = false;
}

void ABoardGamePlayerController::CameraZoomIn()
{
	auto character = (ABoardGameCharacter*)GetPawn();
	auto cameraBoom = character->GetCameraBoom();
	if ((cameraBoom->TargetArmLength - CameraZoomIncreese) < CameraZoomInLimit)
	{
		cameraBoom->TargetArmLength = CameraZoomInLimit;
	}
	else
	{
		cameraBoom->TargetArmLength -= CameraZoomIncreese;
	}
}

void ABoardGamePlayerController::CameraZoomOut()
{
	auto character = (ABoardGameCharacter*)GetPawn();
	auto cameraBoom = character->GetCameraBoom();
	if ((cameraBoom->TargetArmLength + CameraZoomIncreese) > CameraZoomOutLimit)
	{
		cameraBoom->TargetArmLength = CameraZoomOutLimit;
	}
	else
	{
		cameraBoom->TargetArmLength += CameraZoomIncreese;
	}
}
