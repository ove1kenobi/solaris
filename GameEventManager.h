#pragma once
#include "GameEvent.h"

class GameEventManager {
private:
	std::vector<GameEvent> generalEventPool;
	std::vector<GameEvent> coldEventPool;
	std::vector<GameEvent> hotEventPool;

public:
	GameEventManager();
	~GameEventManager();

	void RequestGameEvents(GameEvent *setOfGameEvents, UINT planetType);
};