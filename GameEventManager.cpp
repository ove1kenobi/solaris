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
	default:
	{
		// Normal
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



	srand(time(NULL));
	std::random_shuffle(generalEventPool.begin(), generalEventPool.end());
	std::random_shuffle(coldEventPool.begin(), coldEventPool.end());
	std::random_shuffle(hotEventPool.begin(), hotEventPool.end());
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
		else  type = 2;
		setOfGameEvents[i] = GetAGameEvent(type);
	}
}
