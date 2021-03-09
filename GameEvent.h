#pragma once
#include "Resources.h"
#include <string>	// pch?

struct GameEvent {
	std::string prologue;
	std::string consequence;
	int reward[numberOfResources] = {0};
};