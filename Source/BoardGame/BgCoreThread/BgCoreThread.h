#pragma once

#include "BoardGame.h"
#include <memory>
#include <vector>
#include <GameController.h>

class AGameMode;
class Action;
class Option;

using namespace std;

class BOARDGAME_API FBgCoreThread : public FRunnable, public GameController
{
	static FBgCoreThread* Runnable;

	FRunnableThread* Thread;

	AGameMode* TheGM;

	weak_ptr<Action> CurrAction;

	bool WaitingForChoise;

	vector<shared_ptr<PlayerController>> PlayerControllers;

public:
	//Constructor / Destructor
	FBgCoreThread(AGameMode& GameMode);
	virtual ~FBgCoreThread();

	bool IsFinished() const;

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

	// Begin GameController interface.
	virtual shared_ptr<PlayerController> get_player_controller(uint32_t player_id);
	// End GameController interface

	void EnsureCompletion();

	/*
	Start the thread and the worker from static (easy access)!
	This code ensures only 1 Prime Number thread will be able to run at a time.
	This function returns a handle to the newly started instance.
	*/
	static FBgCoreThread* JoyInit(AGameMode& GameMode);

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();

	static bool IsThreadFinished();

	bool IsWaitingForChoise() { return WaitingForChoise; };

	const vector<shared_ptr<Option>> & GetCurrOptions();

	void ResolveAction(shared_ptr<Action> action);

	void Choose(shared_ptr<Option> option);
};