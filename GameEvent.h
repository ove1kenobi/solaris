#pragma once
#include "Resources.h"
#include "pch.h"

struct GameEvent {
	std::wstring prologue;
	std::wstring consequence;
	Resources preview = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	Resources reward = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};