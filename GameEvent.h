#pragma once
#include "Resources.h"

#include <iostream>

struct GameEvent {
	std::wstring prologue;
	std::wstring consequence;
	Resources reward;
};

GameEvent GetGameEvent(UINT ID);
UINT CreateID(UINT type, UINT index);

extern std::vector< std::vector<GameEvent> > gameEvents;