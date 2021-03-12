#include "pch.h"
#include "GameEventManager.h"


UINT GameEventManager::GetAGameEvent(UINT type)
{
	UINT index = 0, evType = type;

	switch (type)
	{
		case 0:
		{
			// Hot
			if (m_hotEventPool.empty()) {
				RefillGameEvents(type);
			}
			index = m_hotEventPool.back();
			m_hotEventPool.pop_back();
			break;
		}
		case 1:
		{
			// Cold
			if (m_coldEventPool.empty()) {
				RefillGameEvents(type);
			}
			index = m_coldEventPool.back();
			m_coldEventPool.pop_back();
			break;
		}
		case 2:
		{
			// Radioactive
			if (m_radioactiveEventPool.empty()) {
				RefillGameEvents(type);
			}
			index = m_radioactiveEventPool.back();
			m_radioactiveEventPool.pop_back();
			break;
		}
		default:
		{
			// General
			if (m_generalEventPool.empty()) {
				RefillGameEvents(type);
			}
			index = m_generalEventPool.back();
			m_generalEventPool.pop_back();
			break;
		}
	}

	UINT ID = CreateID(evType, index);

	return ID;
}

void GameEventManager::RefillGameEvents(UINT type)
{
	switch (type) 
	{
		case 0: 
		{
			for (unsigned int i = 0; i < gameEvents[0].size(); i++) {
				m_hotEventPool.push_back(i);
			}
			std::random_shuffle(m_hotEventPool.begin(), m_hotEventPool.end());
			break;
		}
		case 1:
		{
			for (unsigned int i = 0; i < gameEvents[1].size(); i++) {
				m_coldEventPool.push_back(i);
			}
			std::random_shuffle(m_coldEventPool.begin(), m_coldEventPool.end());
			break;
		}
		case 2:
		{
			for (unsigned int i = 0; i < gameEvents[2].size(); i++) {
				m_radioactiveEventPool.push_back(i);
			}
			std::random_shuffle(m_radioactiveEventPool.begin(), m_radioactiveEventPool.end());
			break;
		}
		default:
		{
			for (unsigned int i = 0; i < gameEvents[3].size(); i++) {
				m_generalEventPool.push_back(i);
			}
			std::random_shuffle(m_generalEventPool.begin(), m_generalEventPool.end());
			break;
		}
	}
}



GameEventManager::GameEventManager()
{
	for (unsigned int i = 0; i < gameEvents[0].size(); i++) {
		m_hotEventPool.push_back(i);
	}
	for (unsigned int i = 0; i < gameEvents[1].size(); i++) {
		m_coldEventPool.push_back(i);
	}
	for (unsigned int i = 0; i < gameEvents[2].size(); i++) {
		m_radioactiveEventPool.push_back(i);
	}
	for (unsigned int i = 0; i < gameEvents[3].size(); i++) {
		m_generalEventPool.push_back(i);
	}

	srand(time(NULL));
	std::random_shuffle(m_generalEventPool.begin(), m_generalEventPool.end());
	std::random_shuffle(m_coldEventPool.begin(), m_coldEventPool.end());
	std::random_shuffle(m_hotEventPool.begin(), m_hotEventPool.end());
	std::random_shuffle(m_radioactiveEventPool.begin(), m_radioactiveEventPool.end());
}

GameEventManager::~GameEventManager()
{
}

void GameEventManager::RequestGameEvents(UINT setOfGameEvents[3], UINT planetType)
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
