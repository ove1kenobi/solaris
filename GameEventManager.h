#pragma once
#include "GameEvent.h"

class GameEventManager {
private:
	std::vector< std::vector<GameEvent> > m_gameEvents;

	std::vector<int> m_generalEventPool;
	std::vector<int> m_coldEventPool;
	std::vector<int> m_hotEventPool;
	std::vector<int> m_radioactiveEventPool;

	GameEvent GetAGameEvent(UINT type);
	void RefillGameEvents(UINT type);
	void CreateGameEvents();
public:
	GameEventManager();
	~GameEventManager();

	void RequestGameEvents(GameEvent setOfGameEvents[3], UINT planetType);
};