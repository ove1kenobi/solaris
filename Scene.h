#pragma once
#include "Planet.h"
#include "PlayerCamera.h"
#include "Player.h"
//#include "ModelFactory.h"
//#include "Sun.h"
//#include "GameObject"
#include <random>

class Scene
{
private:
	//GameObjects include planets, moons, asteroids and the sun.
	//std::vector<GameObject> m_gameObjects;
	int m_numPlanets;

	Camera m_orthoCamera;
	PlayerCamera m_perspectiveCamera;

	//Player m_player;

	//ModelFactory m_factory;
public:
	Scene();
	~Scene();

	bool init(unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar, float FOVvalue);
	bool update();
};
