
#include "BoardGame.h"
#include "BgCoreThread.h"
#include <turns/actions/options/Option.h>
#include "BgCorePlayerController.h"
#include "BoardGameGameMode.h"
#include <GameFramework/Actor.h>
#include <GameFramework/GameMode.h>
#include <GameController.h>
#include <turns/actions/Action.h>
#include <BgCore.h>

//***********************************************************
//Thread Worker Starts as NULL, prior to being instanced
//		This line is essential! Compiler error without it
FBgCoreThread* FBgCoreThread::Runnable = NULL;
//***********************************************************

FBgCoreThread::FBgCoreThread(AGameMode& GameMode) : TheGM(&GameMode)
{

	WaitingForChoise = false;

	Thread = FRunnableThread::Create(this, TEXT("FBgCoreThread"), true); //windows default = 8mb for thread, could specify more

	// TEMP: adds two player controllers for the bgCore to comunicate with
	PlayerControllers.push_back(make_shared<BgCorePlayerController>(this));
	PlayerControllers.push_back(make_shared<BgCorePlayerController>(this));
}

FBgCoreThread::~FBgCoreThread()
{
	delete Thread;
	Thread = NULL;
}

//Init
bool FBgCoreThread::Init()
{
	//Init the Data 
	NumOfOpts = 0;

	return true;
}

//Run
uint32 FBgCoreThread::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	//While not told to stop this thread 
	//		and not yet finished finding Prime Numbers
	//while (StopTaskCounter.GetValue() == 0 && !IsFinished())
	//{
	//PrimeNumbers->Add(FindNextPrimeNumber());
	//PrimesFoundCount++;

	//***************************************
	//Show Incremental Results in Main Game Thread!

	//	Please note you should not create, destroy, or modify UObjects here.
	//	  Do those sort of things after all thread are completed.

	//	  All calcs for making stuff can be done in the threads
	//	     But the actual making/modifying of the UObjects should be done in main game thread.
	//ThePC->ClientMessage(FString::FromInt(PrimeNumbers->Last()));
	//***************************************

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//prevent thread from using too many resources
	//FPlatformProcess::Sleep(0.01);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//}

	//Run FPrimeNumberWorker::Shutdown() from the timer in Game Thread that is watching
	//to see when FPrimeNumberWorker::IsThreadFinished()

	if (TheGM)
	{
		ABoardGameGameMode* GmPtr = (ABoardGameGameMode*)TheGM;
		GmPtr->core.start(*this);
	}

	return 0;
}

//stop
void FBgCoreThread::Stop()
{
	//StopTaskCounter.Increment();
}

FBgCoreThread* FBgCoreThread::JoyInit(AGameMode& GameMode)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FBgCoreThread(GameMode);
	}
	return Runnable;
}

void FBgCoreThread::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FBgCoreThread::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool FBgCoreThread::IsFinished() const
{
	//TODO
	return true;
}

bool FBgCoreThread::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}

void FBgCoreThread::ResolveAction(shared_ptr<Action> action)
{
	auto options = action->get_options();
	NumOfOpts = options.size();
	WaitingForChoise = true;
	while (WaitingForChoise)
	{
		FPlatformProcess::Sleep(0.05);
	}
}

shared_ptr<PlayerController> FBgCoreThread::get_player_controller(uint32_t player_id)
{
	return PlayerControllers[player_id];
}
