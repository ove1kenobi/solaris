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
		return false;
	}

	//Perspective Camera. Bound to player.
	if (!this->m_perspectiveCamera.init(screenWidth, screenHeight)) {
		//Throw
		return false;
	}

	if (!m_player.Initialize(&m_perspectiveCamera)) {
		//Throw
		return false;
	}

	//Generate sun.
	Sun *sun = new Sun();
	if(!sun->Initialize()){
		//Throw
		return false;
	}

	this->m_gameObjects.push_back(sun);

	//Get the factory to create the planets.
	//this->m_factory = ModelFactory::getInstance();


	//Beh?ver ?ndras pga factory senare.
	//Generate planets.
	using t_clock = std::chrono::high_resolution_clock;
	//std::seed_seq seed = {  };
	
	std::default_random_engine generator(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distributionPlanets(30, 50);
	this->m_numPlanets = distributionPlanets(generator);
	std::uniform_int_distribution<int> distributionRadius(100, 500);
	std::uniform_int_distribution<int> distributionX(-5000, 5000);
	std::uniform_int_distribution<int> distributionY(-5000, 5000);
	std::uniform_int_distribution<int> distributionZ(-5000, 5000);

	/*
	CosmicBody* planetmiddle = new CosmicBody();
	if (!planetmiddle->init(
		0,
		0,
		0,
		50
	))
	{
		//Throw
		return false;
	}
	this->m_gameObjects.push_back(planetmiddle);*/

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
			return false;
		}
		this->m_gameObjects.push_back(planet);
	}

	this->m_gameObjects.push_back(new SpaceShip());

	return true;
}

bool Scene::update(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {
	DirectX::XMFLOAT4 a = {0.0f, 0.0f, 0.0f, 0.0f};
	m_perspectiveCamera.update(DirectX::XMLoadFloat4(&a));
	m_player.update();
	DirectX::XMMATRIX vMatrix = this->m_perspectiveCamera.getVMatrix();
	DirectX::XMMATRIX pMatrix = this->m_perspectiveCamera.getPMatrix();
	
	for (auto r : this->m_gameObjects) {
		r->update(vMatrix, pMatrix, deviceContext);
	}

	//Cull Objects HERE at the end or as response to AskForObjectsEvent? (Emil F)
	return 1;
}