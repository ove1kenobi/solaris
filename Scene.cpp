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


	//Behöver ändras pga factory senare.
	//Generate planets.
	using t_clock = std::chrono::high_resolution_clock;
	//std::seed_seq seed = {  };
	
	std::default_random_engine generator(t_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distributionPlanets(12, 15);
	this->m_numPlanets = distributionPlanets(generator);
	std::uniform_int_distribution<int> distributionRadius(100, 500);
	std::uniform_int_distribution<int> distributionX(-3000, 3000);
	std::uniform_int_distribution<int> distributionY(-300, 300);
	std::uniform_int_distribution<int> distributionZ(1500, 5000);

	for(int i = 0; i < this->m_numPlanets; i++){
		CosmicBody* planet = new CosmicBody();
		if(!planet->init(
			static_cast<float>(distributionX(generator)),
			static_cast<float>(distributionY(generator)),
			static_cast<float>(distributionZ(generator)),
			static_cast<float>(distributionRadius(generator))
			))
		{
			//Throw
			return 0;
		}
		this->m_gameObjects.push_back(planet);
	}
	return 1;
}

bool Scene::update(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {
	DirectX::XMMATRIX vMatrix = this->m_perspectiveCamera.getVMatrix();
	DirectX::XMMATRIX pMatrix = this->m_perspectiveCamera.getPMatrix();
	
	for (auto r : this->m_gameObjects) {
		r->update(vMatrix, pMatrix, deviceContext);
	}

	//Cull Objects HERE at the end or as response to AskForObjectsEvent? (Emil F)
	return 1;
}