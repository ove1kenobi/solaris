#include "pch.h"
#include "Scene.h"

void initPlanet(Planet* planet, Orbit* orbit, std::vector<GameObject*>& gameObjects, size_t id, size_t num, float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo) {
	planet->Initialize(x, y, z, r, xRot, zRot, rotDir, tetherTo, orbit);

	gameObjects[id] = planet;
	gameObjects[id + num] = orbit;
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
		case EventType::DelegateMouseCoordsEvent:
		{
			DelegateMouseCoordsEvent& derivedEvent = static_cast<DelegateMouseCoordsEvent&>(event);
			m_Picking.DoIntersectionTests(derivedEvent.GetXCoord(), derivedEvent.GetYCoord(), m_gameObjects);
			break;
		}
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
	SendRenderObjectsEvent event(&this->m_gameObjects, m_numPlanets);
	EventBuss::Get().Delegate(event);
}

bool Scene::init(unsigned int screenWidth, unsigned int screenHeight, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext) {
	m_pDeviceContext = pDeviceContext;
	EventBuss::Get().AddListener(this, EventType::AskForRenderObjectsEvent, EventType::DelegateMouseCoordsEvent);

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
	
	//Generate sun.
	Sun* sun = new Sun();
	if (!sun->Initialize()) {
		return false;
	}


	//Generator and distributions used for generating planet values.
	using t_clock = std::chrono::high_resolution_clock;
	std::default_random_engine generator(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distributionPlanets(20, 30);
	this->m_numPlanets = distributionPlanets(generator);
	std::uniform_int_distribution<int> distributionRadius(100, 500);
	//World space coordinates
	std::uniform_int_distribution<int> distributionX(0, 90000);
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
	this->m_gameObjects.resize(this->m_numPlanets * 2);
	//Create all the planets using the distributions.
	for(size_t i = 0; i < this->m_numPlanets; ++i){
		Planet* planet = new Planet();
		Orbit* orbit = new Orbit();
		threads.push_back(std::thread(
			initPlanet,
			planet,
			orbit,
			std::ref(this->m_gameObjects),
			i,
			m_numPlanets,
			static_cast<float>(distributionX(generator)),
			static_cast<float>(distributionY(generator)),
			static_cast<float>(distributionZ(generator)),
			static_cast<float>(distributionRadius(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<int>(distributionRotDir(generator)),
			sun
		));
	}
	for (int i = 0; i < this->m_numPlanets; i++) {
		threads[i].join();
	}

	// Push sun to stack
	this->m_gameObjects.push_back(sun);

	//Add the ship to the gameObject vector.
	this->m_gameObjects.push_back(this->m_player.getShip());

	//Add an asteroid to the gameObject vector.
	for (int i = 1; i < 10; ++i) {
		Asteroid* ast = new Asteroid();
		ast->init(DirectX::XMFLOAT3(0.0f + 500.0f * i , 1000.0f + 500.0f * i, 0.0f + 500.0f * i), DirectX::XMFLOAT3(0.0f, .0f, 0.0f));
		m_gameObjects.push_back(ast);
	}

	if (!m_Picking.Initialize())
		return false;

	return true;
}

void Scene::Update() noexcept {
	ImGui::Begin("Asteroid data");
	auto it = m_gameObjects.end();
	for (int i = 1; i < 10; ++i) {
		--it;
		DirectX::XMFLOAT3 c = (*it)->GetCenter();
		DirectX::XMFLOAT3 v = (*it)->GetVelocity();
		DirectX::XMFLOAT3 f = (*it)->GetSumForces();
		float m = (*it)->GetMass();
		ImGui::Text("Center    : (%.00f, %.00f, %.00f)", c.x, c.y, c.z);
		ImGui::Text("Velocity  : (%.00f, %.00f, %.00f)", v.x, v.y, v.z);
		ImGui::Text("Sum forces: (%.00f, %.00f, %.00f)", f.x, f.y, f.z);
		ImGui::Text("Mass      : %.00f", m);
	}
	ImGui::End();
	// Calculate gravity between each pair of GameObjects
	for (size_t i = 0; i < m_gameObjects.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < m_gameObjects.size(); ++j)
		{
			m_gameObjects[i]->CalculateGravity(m_gameObjects[j]);
		}
	}
	//SpaceShip* ship = this->m_player.getShip();
	//for (size_t i = 0; i < m_gameObjects.size(); ++i)
	//{
	//	ship->CalculateGravity(m_gameObjects[i]);
	//}
	//Update the player and all the game objects.
	m_player.update();
	DirectX::XMMATRIX vMatrix = this->m_perspectiveCamera.getVMatrix();
	DirectX::XMMATRIX pMatrix = this->m_perspectiveCamera.getPMatrix();

	for (auto r : this->m_gameObjects) {
		r->update(vMatrix, pMatrix, m_pDeviceContext);
	}
	m_Picking.DisplayPickedObject();
}