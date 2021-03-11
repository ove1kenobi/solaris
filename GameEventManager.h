#pragma once
#include "GameEvent.h"

#include <iostream>

class GameEventManager {
private:
	std::vector<GameEvent> generalEventPool;
	std::vector<GameEvent> coldEventPool;
	std::vector<GameEvent> hotEventPool;
	std::vector<GameEvent> radioactiveEventPool;

	GameEvent GetAGameEvent(UINT type);

public:
	GameEventManager();
	~GameEventManager();

	void RequestGameEvents(GameEvent setOfGameEvents[3], UINT planetType);
};