#pragma once
#include "GameEvent.h"

#include <iostream>

class GameEventManager {
private:
	std::vector<int> m_generalEventPool;
	std::vector<int> m_coldEventPool;
	std::vector<int> m_hotEventPool;
	std::vector<int> m_radioactiveEventPool;

	UINT GetAGameEvent(UINT type);
	void RefillGameEvents(UINT type);
public:
	GameEventManager();
	~GameEventManager();

	void RequestGameEvents(UINT setOfGameEvents[3], UINT planetType);
};