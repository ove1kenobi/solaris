#pragma once
#include "CosmicBody.h"
#include "PlayerCamera.h"
//#include "ModelFactory.h"
//#include "Sun.h"
//#include "Player.h"
#include "GameObject.h"
#include <random>

class Scene
{
private:
	//GameObjects include planets, moons, asteroids and the sun.
	int m_numPlanets;

	Camera m_orthoCamera;

	//Player m_player;

	//ModelFactory m_factory;
public:
	Scene() noexcept;
	~Scene() = default;

	std::vector<CosmicBody> m_gameObjects;

	bool init(unsigned int screenWidth, unsigned int screenHeight);
	bool update();

	PlayerCamera m_perspectiveCamera;
};
