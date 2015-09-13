#pragma once

#include <player/PlayerController.h>

class FBgCoreThread;

class BOARDGAME_API BgCorePlayerController : public PlayerController
{

	FBgCoreThread* Runner;

public:

	BgCorePlayerController(FBgCoreThread* Runner) : Runner(Runner) {}

	virtual void resolve_action(shared_ptr<Action> action);

};