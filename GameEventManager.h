#pragma once
#include "GameEvent.h"

class GameEventManager {
private:
	std::vector<GameEvent> generalEventPool;
	std::vector<GameEvent> coldEventPool;
	std::vector<GameEvent> hotEventPool;

	GameEvent GetAGameEvent(UINT type);

public:
	GameEventManager();
	~GameEventManager();

	void RequestGameEvents(GameEvent setOfGameEvents[3], UINT planetType);
};