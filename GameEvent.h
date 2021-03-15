#pragma once
#include "Resources.h"

struct GameEvent {
	std::wstring prologue;
	std::wstring consequence;
	Resources reward;
	int health;
};

extern std::vector< std::vector<GameEvent> > gameEvents;

GameEvent GetGameEvent(UINT ID);
UINT CreateID(UINT type, UINT index);