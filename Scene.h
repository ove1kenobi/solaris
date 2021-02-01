#pragma once
#include "CosmicBody.h"
#include "Player.h"
//#include "ModelFactory.h"
//#include "Sun.h"
//#include "Player.h"
#include "GameObject.h"
#include <random>

class Scene
{
private:
	//GameObjects include planets, moons, asteroids and the sun.
	std::vector<GameObject*> m_gameObjects;
	int m_numPlanets;

	Camera m_orthoCamera;
	PlayerCamera m_perspectiveCamera;

	//Player m_player;

	//ModelFactory m_factory;
public:
	Scene() noexcept;
	~Scene() = default;

	bool init(unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar, float FOVvalue);
	bool update();
};
