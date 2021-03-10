#include "pch.h"
#include "Scene.h"

void initPlanet(Planet* planet, Orbit* orbit, WaterSphere* waterSphere, std::vector<GameObject*>& gameObjects, std::vector<Planet*>& planets, std::vector<WaterSphere*>& waterSpheres, std::vector<Planet*>& radioactivePlanets, size_t id, size_t num, float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo) {
	float distanceFromSun = std::sqrt(x * x + y * y + z * z);
	//Planet types can be found in planet.h

	planet->Initialize(x, y, z, r, xRot, zRot, rotDir, type, tetherTo, orbit, waterSphere);

	if (type == 2) {
		radioactivePlanets[id] = planet;
	}

	gameObjects[id] = planet;
	gameObjects[id + num] = orbit;
	planets[id] = planet;
	waterSpheres[id] = waterSphere;
}

const std::vector<std::wstring> Scene::RandomizePlanetNames(std::default_random_engine generator) noexcept
{
	//Fill a vector with the names for the planets:
	std::vector<std::wstring> planetNames;
	std::vector<UINT> indexesUsed;
	std::ifstream inFile;
	inFile.open("PlanetNames.txt");
	if (inFile.is_open())
	{
		for (int i{ 0u }; i < m_numPlanets; ++i)
		{
			bool nameOccupied = false;
			std::uniform_int_distribution<UINT> nameIndex(0, 49);
			UINT index = static_cast<UINT>(nameIndex(generator));
			for (unsigned int j{ 0u }; j < indexesUsed.size() && nameOccupied == false; j++)
			{
				if (indexesUsed[j] == index)
				{
					nameOccupied = true;
					//Go back one step to retry with a random number:
					i--;
				}
			}
			if (nameOccupied == false)
			{
				/*Name was not in use, so the line corresponding to the index should be used to fetch
				  the planet name at corresponding line in .txt-file.*/
				indexesUsed.push_back(index);
				inFile.seekg(std::ios::beg);
				for (int i = 0; i < static_cast<int>(index); ++i)
				{
					inFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				std::string planetName;
				std::getline(inFile, planetName);
				std::wstring widePlanetName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(planetName);
				planetNames.push_back(widePlanetName);
			}
		}
		inFile.close();
	}
	else
	{
		//We need a way to send back an error signal:
		planetNames.push_back(L"ERROR");
	}
	return planetNames;
}

Scene::Scene() noexcept
	:	m_numPlanets{ 0 }, m_pDeviceContext{ nullptr }, m_RenderData{ }, m_sun{ nullptr }, m_damageTimer{ 0 }, m_persistentObjEnd{ 0u }
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
	this->m_numPlanets = 15;
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

	//Counters for the different types of planets. (None for hot or cold since they depend on length from sun.
	UINT rC = 0; //radium counter, nr 2
	UINT tC = 0; //titanium counter, nr 3
	UINT sC = 0; //scrap metal counter, nr 4
	UINT nC = 0; //nanotech counter, nr 5
	

	//Randomize and return all planet names to be used:
	std::vector<std::wstring> planetNames = RandomizePlanetNames(generator);
	if (planetNames[0] == L"ERROR")
		return false;

	//Create all the planets using the distributions.
	for(size_t i = 0; i < this->m_numPlanets; ++i){
		bool changeType = false;
		Planet* planet = new Planet(planetNames[i]);
		Orbit* orbit = new Orbit();
		WaterSphere* waterSphere = new WaterSphere();
		xCoord += static_cast<float>(distributionX(generator));

		UINT type = static_cast<UINT>(distributionType(generator));
		//If the small chance that no radioactive gets generated by the 10th planet.
		if (i == 10 && rC == 0) {
			type = 2;
			rC++;
		}
		else {
			if (xCoord < 5000.0f) {
				type = 0;
			}
			else if (xCoord > 13000.0f) {
				type = 1;
			}
			else if (type == 2) {
				if (rC == 2) { //Max 2 radioactive
					changeType = true;
				}
				else {
					rC++;
				}
			}
			else if (type == 3) {
				if (tC == 3) { //Max 3 of the other planets
					changeType = true;
				}
				else {
					tC++;
				}
			}
			else if (type == 4) {
				if (sC == 3) {
					changeType = true;
				}
				else {
					sC++;
				}
			}
			else if (type == 5) {
				if (nC == 3) {
					changeType = true;
				}
				else {
					nC++;
				}
			}

			if (changeType) {
				if (rC != 2) {
					type = 2;
					rC++;
				}
				else if (tC != 3) {
					type = 3;
					tC++;
				}
				else if (sC != 3) {
					type = 4;
					sC++;
				}
				else if (nC != 3) {
					type = 5;
					nC++;
				}
			}
		}
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
			type,
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
	//Update the player and all the game objects.
	size_t num = m_gameObjects.size() - m_persistentObjEnd;
	if (m_player.update(m_planets) &&  num < 30)
	{
		//Generator and distributions used for generating planet values.
		using t_clock = std::chrono::high_resolution_clock;
		std::default_random_engine gen(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
		std::uniform_real_distribution<float> dist(200.0f, 900.0f);
		std::uniform_real_distribution<float> adj(0.1f, 10.0f);

		// Add an asteroid to the gameObject vector.
		GameObject* ship = m_player.getShip();
		DirectX::XMFLOAT3 velocity = norm(ship->GetVelocity());
		// Start pos in front of spaceship, and randomise it slightly
		DirectX::XMFLOAT3 pos = ship->GetCenter() + velocity * 4000.0f;
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

	DirectX::XMFLOAT4X4 vMatrix = m_perspectiveCamera.getVMatrix();
	DirectX::XMFLOAT4X4 pMatrix = m_perspectiveCamera.getPMatrix();

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

	
	bool radioactiveUpgrade = false; //Temporary, change to return value of function
	bool coldUpgrade = m_player.getShip()->IsUpgraded(SpaceShip::cold);
	bool hotUpgrade = m_player.getShip()->IsUpgraded(SpaceShip::hot);
	//Only waste time on updating the damage timer if we do not have all of these upgrades.
	//This is correct.
	if (!coldUpgrade || !hotUpgrade || !radioactiveUpgrade) {
		m_damageTimer += m_time.DeltaTime();
	}

	//Update player health
	//Sun
	if (!coldUpgrade || !hotUpgrade) {
		DirectX::XMFLOAT3 playerCenter = m_player.getShip()->getCenter();
		DirectX::XMFLOAT3 sunCenter = m_sun->GetCenter();
		float sunDist = distance(sunCenter, playerCenter);
		if ((sunDist < 5000.0f || sunDist > 13000.0f) && m_damageTimer > 1.0f) {
			m_player.UpdateHealth(-5);
			//Send event to UI so that we can tell the player that we are too far away / too close to the sun.
			m_damageTimer = 0.0f;
		}
	}

	//Radioactive planets
	if (!radioactiveUpgrade) {
		DirectX::XMFLOAT3 playerCenter = m_player.getShip()->getCenter();
		for (auto r : m_radioactivePlanets) {
			float planetDist = distance(r->GetCenter(), playerCenter);
			if (planetDist < 1000.0f && m_damageTimer > 1.0f) {
				m_player.UpdateHealth(-5);
				//Send event to UI so that we can tell the player that we are too close to the sun.
				m_damageTimer = 0.0f;
			}
		}
	}

#if defined(DEBUG) | defined(_DEBUG)
	int health = m_player.GetHealth();

	ImGui::Begin("Health");
	ImGui::Text("%d", health);
	ImGui::End();

	ImGui::Begin("Asteroids");
	ImGui::Text("gameObjects: %d", m_gameObjects.size());
	ImGui::Text("Asteroids  : %d", m_gameObjects.size() - m_persistentObjEnd - 1);
	ImGui::End();
#endif

	if (m_player.GetHealth() <= 0) {
		//game over
	}
}