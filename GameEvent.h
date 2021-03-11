#pragma once
#include "Resources.h"

struct GameEventID {
	int planetType;
	int columnID;
};

struct GameEvent {
	std::string prologue;
	std::string consequence;
	Inventory reward;
};

extern std::vector< std::vector<GameEvent> > gameEvents;