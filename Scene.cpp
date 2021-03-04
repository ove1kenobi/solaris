#include "pch.h"
#include "Scene.h"

void initPlanet(Planet* planet, Orbit* orbit, WaterSphere* waterSphere, std::vector<GameObject*>& gameObjects, std::vector<Planet*>& planets, std::vector<WaterSphere*>& waterSpheres, std::vector<Planet*>& radioactivePlanets, size_t id, size_t num, float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo) {
	float distanceFromSun = std::sqrt(x * x + y * y + z * z);
	//Planet types can be found in planet.h
	if (distanceFromSun < 5000.0f)
		type = 0;
	else if (distanceFromSun > 10000.0f)
		type = 1;

	planet->Initialize(x, y, z, r, xRot, zRot, rotDir, type, tetherTo, orbit, waterSphere);

	if (type == 2) {
		radioactivePlanets[id] = planet;
	}

	gameObjects[id] = planet;
	gameObjects[id + num] = orbit;
	planets[id] = planet;
	waterSpheres[id] = waterSphere;
}

Scene::Scene() noexcept
	:	m_numPlanets{ 0 }, m_pDeviceContext{ nullptr }, m_RenderData{ }, m_sun{ nullptr }, m_damageTimer{ 0 }
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
	m_sun = new Sun();
	if (!m_sun->Initialize()) {
		return false;
	}
	
	//Generator and distributions used for generating planet values.
	using t_clock = std::chrono::high_resolution_clock;
	std::default_random_engine generator(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distributionPlanets(10, 15);
	this->m_numPlanets = distributionPlanets(generator);
	std::uniform_int_distribution<int> distributionRadius(5, 50);
	//World space coordinates
	//X IS USED AS AN ADDITATIVE COMPONENT
	std::uniform_int_distribution<int> distributionX(600, 1000);
	std::uniform_int_distribution<int> distributionY(0, 0);
	std::uniform_int_distribution<int> distributionZ(0, 0);
	//Needs to be radians
	std::uniform_real_distribution<float> distributionXZRot(static_cast<float>(-M_PI_2), static_cast<float>(M_PI_2));
	//negative rotation direction if 0.
	std::uniform_int_distribution<int> distributionRotDir(0, 1);
	std::uniform_int_distribution<UINT> distributionType(2, 5);

	ModelFactory::Get().PreparePlanetDisplacement();
	std::vector<std::thread> threads;

	float xCoord = 2500.0f;
	this->m_gameObjects.resize(this->m_numPlanets * 2);
	this->m_planets.resize(this->m_numPlanets);
	this->m_waterSpheres.resize(this->m_numPlanets);
	this->m_radioactivePlanets.resize(this->m_numPlanets);
	//Create all the planets using the distributions.
	for(size_t i = 0; i < this->m_numPlanets; ++i){
		Planet* planet = new Planet();
		Orbit* orbit = new Orbit();
		WaterSphere* waterSphere = new WaterSphere();
		xCoord += static_cast<float>(distributionX(generator));
		threads.push_back(std::thread(
			initPlanet,
			planet,
			orbit,
			waterSphere,
			std::ref(this->m_gameObjects),
			std::ref(this->m_planets),
			std::ref(this->m_waterSpheres),
			std::ref(this->m_radioactivePlanets),
			i,
			m_numPlanets,
			xCoord,
			static_cast<float>(distributionY(generator)),
			static_cast<float>(distributionZ(generator)),
			static_cast<float>(distributionRadius(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<int>(distributionRotDir(generator)),
			static_cast<UINT>(distributionType(generator)),
			m_sun
		));
	}
	for (int i = 0; i < this->m_numPlanets; i++) {
		threads[i].join();
	}

	//Remove empty spots in the vector.
	size_t j = 0;
	for (size_t i = 0; i < m_radioactivePlanets.capacity(); i++) {
		if (m_radioactivePlanets[j] == NULL) {
			m_radioactivePlanets.erase(m_radioactivePlanets.begin() + j);
		}
		else {
			j++;
		}
	}
	
	// Push sun to stack
	this->m_gameObjects.push_back(m_sun);
	//Add the ship to the gameObject vector.
	this->m_gameObjects.push_back(this->m_player.getShip());

	if (!m_Picking.Initialize())
		return false;
	if (!m_FrustumCulling.Initialize(m_perspectiveCamera))
		return false;

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
	//Cull the objects, update the RenderData-struct for use in forward renderer:
	m_FrustumCulling.CullObjects(m_gameObjects, m_perspectiveCamera, m_RenderData);
	m_RenderData.totalNrOfPlanets = m_numPlanets;
	m_RenderData.waterSpheres = &m_waterSpheres;

	m_Picking.DisplayPickedObject();

	//Update player health
	//Sun
	m_damageTimer += m_time.DeltaTime();
	DirectX::XMFLOAT3 sunCenter = m_sun->GetCenter();
	DirectX::XMFLOAT3 playerCenter = m_player.getShip()->getCenter();
	float sunDist = distance(sunCenter, playerCenter);
	if ((sunDist < 5000.0f || sunDist > 10000.0f) && m_damageTimer > 1.0f ) {
		m_player.UpdateHealth(-5);
		//Send event to UI so that we can tell the player that we are too far away / too close to the sun.
		m_damageTimer = 0.0f;
	}
	
	//Radioactive planets
	for (auto r : m_radioactivePlanets) {
		float planetDist = distance(r->GetCenter(), playerCenter);
		if (planetDist < 1000.0f && m_damageTimer > 1.0f) {
			m_player.UpdateHealth(-5);
			//Send event to UI so that we can tell the player that we are too close to the sun.
			m_damageTimer = 0.0f;
		}
	}
	
#if defined(DEBUG) | defined(_DEBUG)
	int health = m_player.GetHealth();

	ImGui::Begin("Health");
	ImGui::Text("%d", health);
	ImGui::End();
#endif

	if (m_player.GetHealth() <= 0) {
		//game over
	}
}