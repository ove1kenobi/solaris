#pragma once
#include "CosmicBody.h"
#include "PlayerCamera.h"
//#include "ModelFactory.h"
//#include "Sun.h"
//#include "Player.h"
#include "GameObject.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include <random>

class Scene : public EventPublisher, IEventListener
{
private:
	//GameObjects include planets, moons, asteroids and the sun.
	int m_numPlanets;

	Camera m_orthoCamera;

	//Player m_player;

	//ModelFactory m_factory;
public:
	Scene() noexcept;
	~Scene();

	std::vector<GameObject*> m_gameObjects;

	bool init(unsigned int screenWidth, unsigned int screenHeight);
	bool update();

	void OnEvent(IEvent& event) noexcept;
	void sendObjects();

	PlayerCamera m_perspectiveCamera;
};
