#include "pch.h"
#include "Scene.h"

void initPlanet(Planet* planet, Orbit* orbit, WaterSphere* waterSphere, std::vector<GameObject*>& gameObjects, std::vector<Planet*>& planets, std::vector<WaterSphere*>& waterSpheres, size_t id, size_t num, float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo) {
	planet->Initialize(x, y, z, r, xRot, zRot, rotDir, type, tetherTo, orbit, waterSphere);

	gameObjects[id] = planet;
	gameObjects[id + num] = orbit;
	planets[id] = planet;
	waterSpheres[id] = waterSphere;
}

Scene::Scene() noexcept
	:	m_numPlanets{ 0 }, m_pDeviceContext{ nullptr }, m_RenderData{ }, m_persistentObjEnd{ 0u }
{

}

Scene::~Scene() {
	for (auto r : this->m_gameObjects) {
		delete r;
	}
	for (auto r : this->m_waterSpheres) {
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
	SendRenderObjectsEvent event(&m_RenderData);
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
	std::uniform_int_distribution<int> distributionRadius(5, 50);
	//World space coordinates
	std::uniform_int_distribution<int> distributionX(3000, 10000);
	std::uniform_int_distribution<int> distributionY(0, 0);
	std::uniform_int_distribution<int> distributionZ(0, 0);
	//Needs to be radians
	std::uniform_real_distribution<float> distributionXZRot(static_cast<float>(-M_PI_2), static_cast<float>(M_PI_2));
	//negative rotation direction if 0.
	std::uniform_int_distribution<int> distributionRotDir(0, 1);
	std::uniform_int_distribution<UINT> distributionType(2, 5);

	ModelFactory::Get().PreparePlanetDisplacement();
	std::vector<std::thread> threads;

	this->m_gameObjects.resize(this->m_numPlanets * 2);
	this->m_planets.resize(this->m_numPlanets);
	this->m_waterSpheres.resize(this->m_numPlanets);
	//Create all the planets using the distributions.
	for(size_t i = 0; i < this->m_numPlanets; ++i){
		Planet* planet = new Planet();
		Orbit* orbit = new Orbit();
		WaterSphere* waterSphere = new WaterSphere();
		threads.push_back(std::thread(
			initPlanet,
			planet,
			orbit,
			waterSphere,
			std::ref(this->m_gameObjects),
			std::ref(this->m_planets),
			std::ref(this->m_waterSpheres),
			i,
			m_numPlanets,
			static_cast<float>(distributionX(generator)),
			static_cast<float>(distributionY(generator)),
			static_cast<float>(distributionZ(generator)),
			static_cast<float>(distributionRadius(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<int>(distributionRotDir(generator)),
			static_cast<UINT>(distributionType(generator)),
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
	Asteroid* ast = new Asteroid();
	ast->init({ 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f }, m_player.getShip());
	this->m_gameObjects.push_back(ast);

	// Finished creating persistent objects
	// The rest of m_gameObjects contain eventual transient objects
	m_persistentObjEnd = m_gameObjects.size() - 1;

	if (!m_Picking.Initialize())
		return false;
	if (!m_FrustumCulling.Initialize(m_perspectiveCamera))
		return false;

	return true;
}

void Scene::AddGameObject(GameObject* obj)
{
	m_gameObjects.push_back(obj);
}

void Scene::RemoveGameObject(GameObject* obj)
{
	auto i = m_gameObjects.begin() + m_persistentObjEnd;

	while (++i < m_gameObjects.end() && *i != obj);
	if (i != m_gameObjects.end())
	{
		delete* i;
		m_gameObjects.erase(i);
	}
}


void Scene::Update() noexcept {
	//Generator and distributions used for generating planet values.
	using t_clock = std::chrono::high_resolution_clock;
	std::default_random_engine gen(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> dist(200.0f, 900.0f);
	std::uniform_real_distribution<float> adj(0.1f, 10.0f);

	//Update the player and all the game objects.
	size_t num = m_gameObjects.size() - m_persistentObjEnd;
	if (m_player.update() &&  num < 30)
	{
		// Add an asteroid to the gameObject vector.
		GameObject* ship = m_player.getShip();
		DirectX::XMFLOAT3 velocity = norm(ship->GetVelocity());
		// Start pos in front of spaceship, and randomise it slightly
		DirectX::XMFLOAT3 pos = ship->GetCenter() + velocity * 3000.0f;
		pos = pos + DirectX::XMFLOAT3(dist(gen), dist(gen), dist(gen));
		// Give asteroid a random velocity in the general direction of spaceship
		velocity = velocity * DirectX::XMFLOAT3(-adj(gen), -adj(gen), -adj(gen));
		Asteroid* ast = new Asteroid();
		ast->init(pos, velocity, ship);
		m_gameObjects.push_back(ast);
	}

	ImGui::Begin("Asteroids Data");
	for (size_t i = m_persistentObjEnd + 1; i < m_gameObjects.size(); ++i)
	{
		DirectX::XMFLOAT3 c = m_gameObjects[i]->GetCenter();
		ImGui::Text("%d - center: (%.0f, %.0f, %.0f)", i, c.x, c.y, c.z);
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

	DirectX::XMMATRIX vMatrix = this->m_perspectiveCamera.getVMatrix();
	DirectX::XMMATRIX pMatrix = this->m_perspectiveCamera.getPMatrix();

	GameObject* del = nullptr;
	std::vector<GameObject*> remove;
	for (auto r : this->m_gameObjects) {
		del = r->update(vMatrix, pMatrix, m_pDeviceContext);
		if (del) remove.push_back(del);
	}
	for (auto r : remove) {
		RemoveGameObject(r);
	}
	//Cull the objects, update the RenderData-struct for use in forward renderer:
	m_FrustumCulling.CullObjects(m_gameObjects, m_perspectiveCamera, m_RenderData);
	m_RenderData.totalNrOfPlanets = m_numPlanets;
	m_RenderData.waterSpheres = &m_waterSpheres;

	m_Picking.DisplayPickedObject();

	ImGui::Begin("Asteroids");
	ImGui::Text("gameObjects: %d", m_gameObjects.size());
	ImGui::Text("Asteroids  : %d", m_gameObjects.size() - m_persistentObjEnd - 1);
	ImGui::End();
}