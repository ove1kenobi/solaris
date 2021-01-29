#include "Scene.h"

Scene::Scene() noexcept
	:	m_numPlanets{ 0 }
{

}

bool Scene::init(unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar, float FOVvalue) {
	//Orthographic camera. Over the sun.
	Camera orthoCamera;
	this->m_orthoCamera = orthoCamera;
	if (!this->m_orthoCamera.init(screenWidth, screenHeight, screenNear, screenFar, FOVvalue)) {

	}

	//Perspective Camera. Bound to player.
	PlayerCamera playerCamera;
	this->m_perspectiveCamera = playerCamera;
	if (!this->m_perspectiveCamera.init(screenWidth, screenHeight, screenNear, screenFar, FOVvalue)) {
		//Throw
		return 0;
	}

	//Generate sun.
	/*
	Sun sun;
	if(!sun.init()){
		//Throw
		return -1;
	}

	this->m_gameObjects.push_back(sun);
	*/

	//Get the factory to create the planets.
	//this->m_factory = ModelFactory::getInstance();


	//Behöver ändras pga factory senare.
	//Generate planets.
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distributionPlanets(12, 15);
	this->m_numPlanets = distributionPlanets(generator);
	std::uniform_int_distribution<int> distributionRadius(100, 500);
	
	for(int i = 0; i < this->m_numPlanets; i++){
		CosmicBody planet;
		if(!planet.init(static_cast<float>(i * 1000), 0, 0, static_cast<float>(distributionRadius(generator)))){
			//Throw
			return 0;
		}
		this->m_gameObjects.push_back(&planet);
	}
	

	return 1;
}

bool Scene::update() {
	/*
	for (auto r : this->m_gameObjects) {
		r.update();
	}
	*/
	return 1;
}