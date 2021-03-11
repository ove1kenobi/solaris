#include "pch.h"
#include "GameEventManager.h"


GameEvent GameEventManager::GetAGameEvent(UINT type)
{
	GameEvent gameEv;

	switch (type)
	{
	case 0:
	{
		// Hot
		if (!hotEventPool.empty()) {
			gameEv = hotEventPool.back();
			hotEventPool.pop_back();
		}
		else {
			// handle special case
		}
		break;
	}
	case 1:
	{
		// Cold
		if (!coldEventPool.empty()) {
			gameEv = coldEventPool.back();
			coldEventPool.pop_back();
		}
		else {
			// handle special case
		}
		break;
	}
	case 2:
	{
		// Radioactive
		if (!radioactiveEventPool.empty()) {
			gameEv = radioactiveEventPool.back();
			radioactiveEventPool.pop_back();
		}
		else {
			// handle special case
		}
		break;
	}
	default:
	{
		// General
		if (!generalEventPool.empty()) {
			gameEv = generalEventPool.back();
			generalEventPool.pop_back();
		}
		else {
			// handle special case
		}
		break;
	}
	}

	return gameEv;
}



GameEventManager::GameEventManager()
{

	GameEvent general[] = { 
		{
		"Now this is the story all about how My life got flipped, turned upside down",
		"In west Philadelphia born and raised On the playground where I spent most of my days Chilling out, maxing, relaxing all cool",
		{10, 0, 0, 0, 0, 5, 0, 0}
		},
		{
		"Test",
		"Bad choice",
		{10, 5, -10, 0, 0, 0, 0, -1}
		},
		{
		"Raid: Shadow Legends",
		"Throw down the gauntlet and brawl with other players in fierce arena battles. Smoke your competition, climb Tiers, and earn valuable Rewards.",
		{100, 100, 100, 100, 100, 10, 10, 10}
		},
		{
		"Prevent wildfires",
		"Smokey The Bear liked that",
		{100, -100, 10, 0, 0, -2, 0, 0}
		}
	};

	GameEvent hot[] = {
		{
		"Test 1",
		"result",
		{0, 0, 0, 0, 0, 5, 0, 0}
		},
		{
		"Test 2",
		"result",
		{10, 5, 0, 0, 0, 1, 0, 0}
		},
		{
		"Test 3",
		"result",
		{100, 100, 100, 100, 100, 10, 10, 10}
		},
		{
		"Test 4",
		"result",
		{100, -100, 100, 0, 0, 5, 0, 0}
		}
	};

	GameEvent cold[] = {
		{
		"Test 5",
		"result",
		{0, 0, 0, 0, 0, 0, 0, 5}
		},
		{
		"Test 6",
		"result",
		{10, 5, 0, 0, 0, 0, 0, 1}
		},
		{
		"Test 7",
		"result",
		{100, 100, 100, 100, 100, 10, 10, 10}
		},
		{
		"Test 8",
		"result",
		{100, -100, 100, 0, 0, 0, 0, 5}
		}
	};

	GameEvent radioactive[] = {
		{
		"Test 9",
		"result",
		{0, 0, 0, 0, 0, 0, 0, 5}
		},
		{
		"Test 10",
		"result",
		{10, 5, 0, 0, 0, 0, 0, 1}
		},
		{
		"Test 11",
		"result",
		{100, 100, 100, 100, 100, 10, 10, 10}
		},
		{
		"Test 12",
		"result",
		{100, -100, 100, 0, 0, 0, 0, 5}
		}
	};

	for (unsigned int i = 0; i < 4; i++) {
		generalEventPool.push_back(general[i]);
	}
	for (unsigned int i = 0; i < 4; i++) {
		hotEventPool.push_back(hot[i]);
	}
	for (unsigned int i = 0; i < 4; i++) {
		coldEventPool.push_back(cold[i]);
	}
	for (unsigned int i = 0; i < 4; i++) {
		radioactiveEventPool.push_back(radioactive[i]);
	}

	srand(time(NULL));
	std::random_shuffle(generalEventPool.begin(), generalEventPool.end());
	std::random_shuffle(coldEventPool.begin(), coldEventPool.end());
	std::random_shuffle(hotEventPool.begin(), hotEventPool.end());
	std::random_shuffle(radioactiveEventPool.begin(), radioactiveEventPool.end());
}

GameEventManager::~GameEventManager()
{
}

void GameEventManager::RequestGameEvents(GameEvent setOfGameEvents[3], UINT planetType)
{
	setOfGameEvents[0] = GetAGameEvent(planetType);

	bool rnd = 0;
	int type = 2;
	for (int i = 1; i < 3; i++) {
		rnd = rand() % 2;

		if (rnd) type = planetType;
		else  type = 3;
		setOfGameEvents[i] = GetAGameEvent(type);
	}
}
