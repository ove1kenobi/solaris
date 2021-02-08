#include "pch.h"
#include "Scene.h"

Scene::Scene() noexcept
	:	m_numPlanets{ 0 }, m_pDeviceContext{ nullptr }
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

const std::string Scene::GetDebugName() const noexcept
{
	return "Scene";
}

//Send gameObjects for rendering after being asked.
void Scene::sendObjects() {
	SendRenderObjectsEvent event(&this->m_gameObjects);
	EventBuss::Get().Delegate(event);
}

bool Scene::init(unsigned int screenWidth, unsigned int screenHeight, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) {
	m_pDeviceContext = pDeviceContext;
	EventBuss::Get().AddListener(this, EventType::AskForRenderObjectsEvent);
	//Orthographic camera. Over the sun.
	if (!this->m_orthoCamera.init(screenWidth, screenHeight, 1000)) {
		return false;
	}

	//Perspective Camera. Bound to player.
	if (!this->m_perspectiveCamera.init(screenWidth, screenHeight)) {
		return false;
	}

	if (!m_player.Initialize(&m_perspectiveCamera)) {
		return false;
	}

	//Generate sun.
	Sun *sun = new Sun();
	if(!sun->Initialize()){
		return false;
	}

	this->m_gameObjects.push_back(sun);

	//Get the factory to create the planets.
	//this->m_factory = ModelFactory::getInstance();


	
	//Generate planets.
	using t_clock = std::chrono::high_resolution_clock;
	std::default_random_engine generator(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distributionPlanets(30, 50);
	this->m_numPlanets = distributionPlanets(generator);

	std::uniform_int_distribution<int> distributionRadius(100, 500);
	std::uniform_int_distribution<int> distributionX(-5000, 5000);
	std::uniform_int_distribution<int> distributionY(-5000, 5000);
	std::uniform_int_distribution<int> distributionZ(-5000, 5000);
	std::uniform_int_distribution<int> distributionXZRot(-30, 30);
	/*
	CosmicBody* planetmiddle = new CosmicBody();
	if (!planetmiddle->init(
		0,
		0,
		0,
		50,
		5,
		0
	))
	{
		//Throw
		return false;
	}
	this->m_gameObjects.push_back(planetmiddle);
	*/
	for(int i = 0; i < this->m_numPlanets; i++){
		CosmicBody* planet = new CosmicBody();
		if(!planet->init(
			static_cast<float>(distributionX(generator)),
			static_cast<float>(distributionY(generator)),
			static_cast<float>(distributionZ(generator)),
			static_cast<float>(distributionRadius(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<float>(distributionXZRot(generator))
			))
		{
			//Throw
			return false;
		}
		this->m_gameObjects.push_back(planet);
	}

	this->m_gameObjects.push_back(this->m_player.getShip());

	return true;
}


void Scene::Update() noexcept {
	m_player.update();
	DirectX::XMMATRIX vMatrix = this->m_perspectiveCamera.getVMatrix();
	DirectX::XMMATRIX pMatrix = this->m_perspectiveCamera.getPMatrix();

	for (auto r : this->m_gameObjects) {
		r->update(vMatrix, pMatrix, m_pDeviceContext);
	}
#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Game Objects");
	for (unsigned int i{ 0u }; i < m_gameObjects.size(); i++)
	{
		ImGui::Text("Game Object #%d", i + 1);
		ImGui::Text("Center: (%.0f, %.0f, %.0f)", m_gameObjects[i]->GetCenter().x, m_gameObjects[i]->GetCenter().y, m_gameObjects[i]->GetCenter().z);
	}
	ImGui::End();
#endif
	//Cull Objects HERE at the end or as response to AskForObjectsEvent? (Emil F)
}