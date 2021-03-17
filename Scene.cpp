#include "pch.h"
#include "Scene.h"

void initPlanet(Planet* planet, Orbit* orbit, WaterSphere* waterSphere, std::vector<GameObject*>& gameObjects, std::vector<Planet*>& planets, std::vector<WaterSphere*>& waterSpheres, std::vector<Planet*>& radioactivePlanets, size_t id, size_t num, float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo) {
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
	:	m_numPlanets{ 0 }, m_pDeviceContext{ nullptr }, m_RenderData{ }, m_sun{ nullptr }, m_damageTimer{ 1.1f }, m_persistentObjEnd{ 0u }, m_nextAstroSpawnTime{ 0.0 }, m_IsInvincible{ false },
	m_ElapsedTime{ 0.0f }
{

}

Scene::~Scene() {
	for (auto r : this->m_gameObjects) {
		delete r;
	}
	for (auto r : this->m_waterSpheres) {
		delete r;
	}
	EventBuss::Get().RemoveListener(this, EventType::AskForRenderObjectsEvent);
	EventBuss::Get().RemoveListener(this, EventType::DelegateMouseCoordsEvent);
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
	
	//Needs to be radians
	std::uniform_real_distribution<float> distributionXZRot(static_cast<float>(-M_PI_2), static_cast<float>(M_PI_2));
	//negative rotation direction if 0.
	std::uniform_int_distribution<int> distributionBool(0, 1);
	std::uniform_int_distribution<UINT> distributionType(2, 5);

	ModelFactory::Get().PreparePlanetDisplacement();
	std::vector<std::thread> threads;

	float currentDist = 1500.0f;
	int currentMaxDiff = 1500;

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

		//dist IS USED AS AN ADDITATIVE COMPONENT
		std::uniform_int_distribution<int> distributionDist(currentMaxDiff - 200, currentMaxDiff);
		currentMaxDiff -= 50;
		currentDist += static_cast<float>(distributionDist(generator));

		UINT type = static_cast<UINT>(distributionType(generator));
		//If the small chance that no radioactive gets generated by the 10th planet.
		if (i == 10 && rC == 0) {
			type = 2;
			rC++;
		}
		else {
			if (currentDist < 6000.0f) {
				type = 0;
			}
			else if (currentDist > 15000.0f) {
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
			//Coordinates get randomized in cosmicbody.cpp now instead, because of orbits. So we only need the distance. Sending as x-coord.
			currentDist,
			0.0f,
			0.0f,
			static_cast<float>(distributionRadius(generator)),
			static_cast<float>(distributionXZRot(generator)),
			static_cast<float>(distributionXZRot(generator)),
			//What way it rotates.
			static_cast<int>(distributionBool(generator)),
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
	
	//Add flavour text.
	for (size_t i = 0; i < m_planets.size(); i++) {
		UINT type = m_planets[i]->GetPlanetType();
		if (type != 0 && type != 1 && type != 2) {
			m_planets[i]->SetPlanetFlavourText(m_flavourTexts.GetText(planetType::normal));
		}
		else {
			switch (type) {
			case 0:
				m_planets[i]->SetPlanetFlavourText(m_flavourTexts.GetText(planetType::warm));
				break;
			case 1:
				m_planets[i]->SetPlanetFlavourText(m_flavourTexts.GetText(planetType::cold));
				break;
			case 2:
				m_planets[i]->SetPlanetFlavourText(m_flavourTexts.GetText(planetType::radio));
			}
		}
	}

	//Go through the planets and find a suitable one to start at.
	std::uniform_int_distribution<int> distributionStartPosition(2, 5);
	UINT counter = 0;
	UINT startPos = distributionStartPosition(generator);
	for (size_t i = 0; i < m_planets.size(); i++) {
		UINT type = m_planets[i]->GetPlanetType();
		if (type != 0 && type != 1 && type != 2) {
			counter++;
			if (counter == startPos) {
				DirectX::XMFLOAT3 newCenter;
				newCenter = m_planets[i]->GetCenter();
				newCenter.y += 600.0f;
				m_player.getShip()->SetCenter(newCenter);
				break;
			}
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

	m_nextAstroSpawnTime = m_time.SinceStart() + 5.0;

	PlaySoundEvent playSoundEvent(SoundID::Stabilizers, true);
	EventBuss::Get().Delegate(playSoundEvent);
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
		if (*i != nullptr)
		{
			delete* i;
			*i = nullptr;
			m_gameObjects.erase(i);
		}
	}
}


void Scene::Update() noexcept {

	//Update the player and all the game objects.
	size_t num = m_gameObjects.size() - m_persistentObjEnd;

	if (m_player.update(m_planets) &&  num < 60 && m_nextAstroSpawnTime < m_time.SinceStart())
	{
		//Generator and distributions used for generating planet values.
		using t_clock = std::chrono::high_resolution_clock;
		std::default_random_engine gen(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
		std::uniform_real_distribution<float> dist(12000.0f, 17000.0f);
		std::normal_distribution<float> adjustPos(0.0f, 750.0f);
		std::uniform_real_distribution<float> adjustVelocity(0.3f, 1.0f);
		std::uniform_real_distribution<long double> next(1.0, 5.0);
		std::uniform_int_distribution<size_t> asteroids(1u, 7u);

		m_nextAstroSpawnTime = m_time.SinceStart() + next(gen);

		// Add some asteroids to the gameObject vector.
		size_t numAstros = asteroids(gen);
		for (size_t n = 0; n < numAstros; n++)
		{
			GameObject* ship = m_player.getShip();
			DirectX::XMFLOAT3 velocity = ship->GetVelocity();
			// Start pos in front of spaceship, and randomise it slightly
			DirectX::XMFLOAT3 pos = ship->GetCenter() + norm(velocity) * dist(gen);
			pos = pos + DirectX::XMFLOAT3(adjustPos(gen), adjustPos(gen), adjustPos(gen));
			// Give asteroid a random velocity in the general direction of spaceship
			velocity = (ship->GetCenter() - pos) * adjustVelocity(gen);
			Asteroid* ast = new Asteroid();
			ast->init(pos, velocity, ship);
			m_gameObjects.push_back(ast);
		}
	}

#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Asteroids Data");
	for (size_t i = m_persistentObjEnd + 1; i < m_gameObjects.size(); ++i)
	{
		DirectX::XMFLOAT3 c = m_gameObjects[i]->GetCenter();
		ImGui::Text("%d - center: (%.0f, %.0f, %.0f)", i, c.x, c.y, c.z);
	}
	ImGui::End();
#endif

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

	//Is Player invincible from asteroids?
	if (m_IsInvincible)
	{
		m_ElapsedTime += static_cast<float>(m_time.DeltaTime());
		if (m_ElapsedTime > m_InvincibilityDuration)
		{
			m_IsInvincible = false;
			m_ElapsedTime = 0.0f;
		}
	}

	CheckForCollisions();
	GameObject* del = nullptr;
	std::vector<GameObject*> remove;
	for (GameObject* astr : this->m_gameObjects) {
		del = astr->update(vMatrix, pMatrix, m_pDeviceContext);
		size_t i = 0;
		while (del == nullptr && i < m_numPlanets)
		{
			CosmicBody* cosmic = static_cast<CosmicBody*>(m_gameObjects[i]);
			float dist = length(astr->GetCenter() - cosmic->GetCenter());
			if (dist < cosmic->GetRadius())
				del = astr;
			else
				i++;
		}
		if (del) remove.push_back(del);
	}
	for (auto r : remove) {
		RemoveGameObject(r);
	}
	//Cull the objects, update the RenderData-struct for use in forward renderer:
	m_FrustumCulling.CullObjects(m_gameObjects, m_perspectiveCamera, m_RenderData);
	m_RenderData.totalNrOfPlanets = m_numPlanets;
	m_RenderData.waterSpheres = &m_waterSpheres;

	m_Picking.DisplayPickedObject(m_player.HasAntennaUpgrade());

	bool coldDamage = false;
	bool heatDamage = false;
	bool radioactiveDamage = false;
	bool radioactiveUpgrade = m_player.getShip()->IsUpgraded(SpaceShip::radProtect);
	bool coldUpgrade = m_player.getShip()->IsUpgraded(SpaceShip::cold);
	bool hotUpgrade = m_player.getShip()->IsUpgraded(SpaceShip::hot);

	//Update player health
	//Sun
	if (!coldUpgrade || !hotUpgrade) {
		DirectX::XMFLOAT3 playerCenter = m_player.getShip()->getCenter();
		DirectX::XMFLOAT3 sunCenter = m_sun->GetCenter();
		float sunDist = distance(sunCenter, playerCenter);
		if (!hotUpgrade && sunDist < 6000.0f) {
			//Player is taking heat damage
			heatDamage = true;
			m_damageTimer += m_time.DeltaTime();
			if (m_damageTimer > 1.0f) {
				m_player.UpdateHealth(-5);
				m_damageTimer = 0.0f;
			}
		}
		if (!coldUpgrade && sunDist > 15000.0f) {
			//Player is taking cold damage
			coldDamage = true;
			m_damageTimer += m_time.DeltaTime();
			if (m_damageTimer > 1.0f) {
				m_player.UpdateHealth(-5);
				m_damageTimer = 0.0f;
			}
		}
		
	}
	

	//Radioactive planets
	if (!radioactiveUpgrade) {
		DirectX::XMFLOAT3 playerCenter = m_player.getShip()->getCenter();
		for (auto r : m_radioactivePlanets) {
			float planetDist = distance(r->GetCenter(), playerCenter);
			if (planetDist < 1000.0f) {
				//Player is taking radioactive damage
				radioactiveDamage = true;
				m_damageTimer += m_time.DeltaTime();
				if (m_damageTimer > 1.0f) {
					m_player.UpdateHealth(-5);
					m_damageTimer = 0.0f;
				}
			}
		}
	}

	//Forward what type of damage the player is taking to the HUD so it knows what to render
	ToggleDamageHUD dH(coldDamage, heatDamage, radioactiveDamage);
	EventBuss::Get().Delegate(dH);

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
}

void Scene::CheckForCollisions() noexcept
{
	//Planets:
	for (auto planets : m_planets)
	{
		if (length(m_player.getShip()->getCenter() - planets->GetCenter()) <= ((m_player.getShip()->GetBoundingSphere().Radius * 10) + planets->GetRadius()))
		{
			PlaySoundEvent soundEvent(SoundID::ShipExplosion, false);
			EventBuss::Get().Delegate(soundEvent);
			m_player.Kill();
		}
	}
	//Sun:
	if (length(m_player.getShip()->GetCenter() - m_sun->GetCenter()) <= ((m_player.getShip()->GetBoundingSphere().Radius * 10) + m_sun->GetRadius()))
	{
		PlaySoundEvent soundEvent(SoundID::ShipExplosion, false);
		EventBuss::Get().Delegate(soundEvent);
		m_player.Kill();
	}

	//Asteroids
	if (m_IsInvincible == false)
	{
		for (size_t i = m_persistentObjEnd + 1; i < m_gameObjects.size(); i++)
		{
			Asteroid* pAsteroid = static_cast<Asteroid*>(m_gameObjects[i]);
			if (length(m_player.getShip()->getCenter() - pAsteroid->GetCenter()) <= (m_player.getShip()->GetBoundingSphere().Radius) + pAsteroid->GetBoundingSphere().Radius)
			{
				//There is a hit:
				m_IsInvincible = true;
				if (!m_player.HasShieldUpgrade())
				{
					m_player.UpdateHealth(-30);
					m_player.getShip()->AddForce(pAsteroid->GetVelocity() * 2000);
					if (m_player.GetHealth() - 30 <= 0)
					{
						PlaySoundEvent soundEvent(SoundID::ShipExplosion, false);
						EventBuss::Get().Delegate(soundEvent);
					}
					else
					{
						PlaySoundEvent soundEvent(SoundID::AsteroidCollision, false);
						EventBuss::Get().Delegate(soundEvent);
					}
				}
				else
				{
					m_player.UpdateHealth(-15);
					m_player.getShip()->AddForce(pAsteroid->GetVelocity());
					if (m_player.GetHealth() - 15 <= 0)
					{
						PlaySoundEvent soundEvent(SoundID::ShipExplosion, false);
						EventBuss::Get().Delegate(soundEvent);
					}
					else
					{
						PlaySoundEvent soundEvent(SoundID::AsteroidCollision, false);
						EventBuss::Get().Delegate(soundEvent);
					}
				}
				pAsteroid->MarkAsDestroyed();
				CameraShakeEvent csEvent(0.3f, 0.1f);
				EventBuss::Get().Delegate(csEvent);
			}
		}
	}
}

int Scene::GetPlayerHealth() {
	return m_player.GetHealth();
}