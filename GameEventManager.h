#pragma once
#include "GameEvent.h"

#include <iostream>

class GameEventManager {
private:
	std::vector<int> m_generalEventPool;
	std::vector<int> m_coldEventPool;
	std::vector<int> m_hotEventPool;
	std::vector<int> m_radioactiveEventPool;

	GameEventID GetAGameEvent(UINT type);

public:
	GameEventManager();
	~GameEventManager();

	void RequestGameEvents(GameEventID setOfGameEvents[3], UINT planetType);
};