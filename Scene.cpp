#include "pch.h"
#include "Scene.h"

void initPlanet(Planet* planet, std::vector<GameObject*>& gameObjects, std::vector<Planet*>& planets, int id, float x, float y, float z, float r, float xRot, float zRot, int rotDir) {
	planet->Initialize(x, y, z, r, xRot, zRot, rotDir);

	gameObjects[id] = planet;
	planets[id] = planet;
}

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

		case EventType::RequestPlanetsEvent:
			DelegatePlanetsEvent event(&this->m_planets);
			EventBuss::Get().Delegate(event);
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
	EventBuss::Get().AddListener(this, EventType::AskForRenderObjectsEvent, EventType::RequestPlanetsEvent);

	//Orthographic camera. Over the sun. Last parameter is how high above the sun.
	if (!this->m_orthoCamera.init(screenWidth, screenHeight, 1000)) {
		return false;
	}

	//Perspective Camera. Bound to player.
	if (!this->m_perspectiveCamera.init(screenWidth, screenHeight)) {
		return false;
	}

	//Create the player
	if (!m_player.Initialize(&m_perspectiveCamera)) {
		return false;
	}
	
	//Generator and distributions used for generating planet values.
	using t_clock = std::chrono::high_resolution_clock;
	std::default_random_engine generator(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distributionPlanets(20, 30);
	this->m_numPlanets = 1/*distributionPlanets(generator)*/;
	std::uniform_int_distribution<int> distributionRadius(100, 500);
	//World space coordinates
	std::uniform_int_distribution<int> distributionX(1000, 1100);
	std::uniform_int_distribution<int> distributionY(0, 0);
	std::uniform_int_distribution<int> distributionZ(0, 0);
	//Needs to be radians
	std::uniform_real_distribution<float> distributionXZRot(static_cast<float>(-M_PI_2), static_cast<float>(M_PI_2));
	//negative rotation direction if 0.
	std::uniform_int_distribution<int> distributionRotDir(0, 1);

	//Planet in the middle for testing.
	/*
	Planet* planetmiddle = new Planet();
	if (!planetmiddle->init(
		0,
		0,
		0,
		50,
		M_PI_2,
		0
	))
	{
		//Throw
		return false;
	}
	this->m_gameObjects.push_back(planetmiddle);
	*/

	ModelFactory::Get().PreparePlanetDisplacement();
	std::vector<std::thread> threads;
	this->m_gameObjects.resize(this->m_numPlanets);
	this->m_planets.resize(this->m_numPlanets);
	//Create all the planets using the distributions.
	for(int i = 0; i < this->m_numPlanets; i++){
		Planet* planet = new Planet();
		threads.push_back(std::thread(
			initPlanet,
			planet,
			std::ref(this->m_gameObjects),
			std::ref(this->m_planets),
			i,
			static_cast<float>(distributionX(generator)),
			static_cast<float>(distributionY(generator)),
			static_cast<float>(distributionZ(generator)),
			static_cast<float>(distributionRadius(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<int>(distributionRotDir(generator))
		));
	}
	
	for (int i = 0; i < this->m_numPlanets; i++) {
		threads[i].join();
	}

	//Generate sun.
	Sun* sun = new Sun();
	if (!sun->Initialize()) {
		return false;
	}
	this->m_gameObjects.push_back(sun);

	//Add the ship to the gameObject vector.
	this->m_gameObjects.push_back(this->m_player.getShip());

	return true;
}

void Scene::Update() noexcept {
	// Calculate gravity between each pair of GameObjects
	SpaceShip* ship = this->m_player.getShip();
	for (size_t i = 0; i < m_gameObjects.size(); ++i)
	{
		ship->CalculateGravity(m_gameObjects[i]);
	}

	//Update the player and all the game objects.
	m_player.update();
	DirectX::XMMATRIX vMatrix = this->m_perspectiveCamera.getVMatrix();
	DirectX::XMMATRIX pMatrix = this->m_perspectiveCamera.getPMatrix();

	for (auto r : this->m_gameObjects) {
		r->update(vMatrix, pMatrix, m_pDeviceContext);
	}
//#if defined(DEBUG) | defined(_DEBUG)
//	Time t;
//	ImGui::Begin("Game Objects");
//	ImGui::Text("Delta Time: %f", t.DeltaTime());
//	for (unsigned int i{ 0u }; i < m_gameObjects.size(); i++)
//	{
//		ImGui::Text("Game Object #%d", i + 1);
//		ImGui::Text("Center: (%.0f, %.0f, %.0f)", m_gameObjects[i]->GetCenter().x, m_gameObjects[i]->GetCenter().y, m_gameObjects[i]->GetCenter().z);
//	}
//	ImGui::End();
//#endif
	//Cull Objects HERE at the end or as response to AskForObjectsEvent? (Emil F)
}