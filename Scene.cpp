#include "Scene.h"

Scene::Scene() noexcept
	:	m_numPlanets{ 0 }
{

}

Scene::~Scene() {
	for (auto r : this->m_gameObjects) {
		delete r;
	}
}

//All events that Scene are listening to.
void Scene::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		case EventType::AskForRenderObjectsEvent:
			sendObjects();
			break;

		default:
			break;
	}
}

//Send gameObjects for rendering after being asked.
void Scene::sendObjects() {
	SendRenderObjectsEvent event(&this->m_gameObjects);
	EventBuss::Get().Delegate(event);
}

bool Scene::init(unsigned int screenWidth, unsigned int screenHeight) {
	EventBuss::Get().AddListener(this, EventType::AskForRenderObjectsEvent);
	//Orthographic camera. Over the sun.
	if (!this->m_orthoCamera.init(screenWidth, screenHeight, 1000)) {
		//Throw
		return 0;
	}

	//Perspective Camera. Bound to player.
	if (!this->m_perspectiveCamera.init(screenWidth, screenHeight)) {
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

	this->m_gameObjects.push_back(&sun);
	*/

	//Get the factory to create the planets.
	//this->m_factory = ModelFactory::getInstance();


	//Beh�ver �ndras pga factory senare.
	//Generate planets.
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distributionPlanets(12, 15);
	this->m_numPlanets = distributionPlanets(generator);
	std::uniform_int_distribution<int> distributionRadius(100, 500);
	
	for(int i = 0; i < this->m_numPlanets; i++){
		CosmicBody* planet = new CosmicBody();
		if(!planet->init(0/*static_cast<float>(i * 1000)*/, 0, 1500, static_cast<float>(distributionRadius(generator)))){
			//Throw
			return 0;
		}
		this->m_gameObjects.push_back(planet);
	}
	return 1;
}

bool Scene::update() {
	/*
	for (auto r : this->m_gameObjects) {
		r.update();
	}
	*/

	//Cull Objects HERE at the end or as response to AskForObjectsEvent? (Emil F)
	return 1;
}