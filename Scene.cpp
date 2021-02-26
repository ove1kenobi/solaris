#include "pch.h"
#include "Scene.h"

void initPlanet(Planet* planet, Orbit* orbit, std::vector<GameObject*>& gameObjects, size_t id, size_t num, float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo) {
	planet->Initialize(x, y, z, r, xRot, zRot, rotDir, tetherTo, orbit);

	gameObjects[id] = planet;
	gameObjects[id + num] = orbit;
}

Scene::Scene() noexcept
	:	m_numPlanets{ 0 }, m_pDeviceContext{ nullptr }, m_CulledData{ }
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
	SendRenderObjectsEvent event(&m_CulledData);
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

	m_FrustumCulling.CullObjects(m_gameObjects, m_perspectiveCamera, m_CulledData);

	m_Picking.DisplayPickedObject();
}