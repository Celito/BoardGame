
#include "BoardGame.h"
#include "BgCorePlayerController.h"
#include "BgCoreThread.h"

void BgCorePlayerController::resolve_action(shared_ptr<Action> action)
{
	Runner->ResolveAction(action);
}