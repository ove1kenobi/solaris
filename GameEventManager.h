#pragma once
#include "GameEvent.h"

class GameEventManager {
private:
	std::vector<GameEvent> gameEvents;
public:
	GameEventManager();
	~GameEventManager();

	void RequestGameEvents(GameEvent *setOfGameEvents);
};