#include "pch.h"
#include "Player.h"

void Player::UpdateRotation()
{
	float yaw = m_mousePosX * (float)m_time.DeltaTime() * m_rotationSpeed;
	float pitch = -m_mousePosY * (float)m_time.DeltaTime() * m_rotationSpeed;
	float roll = -m_mousePosX * (float)m_time.DeltaTime() * m_rotationSpeed;
	// Rotate the camera
	m_camera->OrbitRotation(yaw, pitch);
	// Rotates the ship
	m_ship->AddRotation(yaw, pitch);
	// Tilts the ship in the direction it is moving
	m_ship->SetTilt(-m_mousePosY, -m_mousePosX);
	m_ship->SetForwardVector(m_camera->GetForwardVector());
}

DirectX::XMFLOAT3 Player::CalculateNeededForce(DirectX::XMFLOAT3 desierdVelocity)
{
	// Get the current velocity vector
	DirectX::XMFLOAT3 velocity = m_ship->GetVelocity();
	// Calculate how much force would be required to redirect the ships velocity to the desierd velocity
	DirectX::XMFLOAT3 neededForce = (desierdVelocity - velocity) * m_ship->GetMass();
	// Check if the trusters have enouth power to produce that force
	if (length(neededForce) > m_thrusterForce * (float)m_time.DeltaTime()) {
		neededForce = normalize(neededForce);
		neededForce = neededForce * m_thrusterForce * (float)m_time.DeltaTime();
	}

	return neededForce;
}

int Player::AddToInventory(int currentResource, int resourceToAdd)
{
	int amountToAdd = 0;
	int storageLeft = m_storageCapacity - m_storageUsage;

	if (storageLeft < resourceToAdd) {
		amountToAdd = storageLeft;
		m_storageUsage = m_storageCapacity;
	}
	else if (currentResource + resourceToAdd < 0) {
		amountToAdd = -currentResource;
		m_storageUsage -= currentResource;

	}
	else {
		amountToAdd = resourceToAdd;
		m_storageUsage += resourceToAdd;
	}

	return amountToAdd;
}

DirectX::XMFLOAT3 Player::ConsumeFuel(DirectX::XMFLOAT3 GeneratedPower)
{
	float engineUse = length(GeneratedPower) / m_thrusterForce;
	float storageUse = (float)m_storageUsage / (float)m_storageCapacity;
	m_inventory.fuel -= engineUse * m_engineEfficiency * (1.0f + storageUse);
	if (m_inventory.fuel < 0.0f) {
		m_inventory.fuel = 0.0f;
		GeneratedPower = GeneratedPower * 0.0f;
	}

	return GeneratedPower;
}

void Player::ConsumeOxygen()
{
	m_inventory.oxygen -= m_oxygenConsumption * (float)m_time.DeltaTime();
	if (m_inventory.oxygen < 0.0f) {
		m_inventory.oxygen = 0.0f;
		Kill();
	}
}

Player::Player()
	: m_PlayerInfo{ },
	  m_TetheredToClosestPlanet{ false },
	  m_TetheredDistanceToUphold{ -1.0f, -1.0f, -1.0f },
	  m_HasShieldUpgrade{ false },
	  m_HasAntennaUpgrade{ false }
{
	m_maxHealth = 100;
	m_fuelCapacity = 100.0f;
	m_oxygenCapacity = 100.0f;
	m_storageCapacity = 500;
	m_inventory.health = 100;
	m_inventory.fuel = 100.0f;
	m_inventory.oxygen = 100.0f;
	m_inventory.titanium = 100;
	m_inventory.scrapMetal = 100;
	m_inventory.nanotech = 100;
	m_inventory.plasma = 5;
	m_inventory.radium = 5;
	m_inventory.khionerite = 5;
	m_inventory.science = 15;
	m_storageUsage = 450;
	m_oxygenConsumption = 0.17f;
	m_engineEfficiency = 1.0f;

	m_moveForwards = false;
	m_moveBackwards = false;
	m_stopMovement = false;
	m_playerControlsActive = true;
	m_stabilizerActive = true;
	m_mousePosX = 0.0f;
	m_mousePosY = 0.0f;

	m_ship = nullptr;
	m_camera = nullptr;
	m_rotationSpeed = 1.0f;

	m_topSpeed = nullptr;
	m_desiredSpeed = 0.0f;
	m_thrusterForce = 500000.0f;
}

Player::~Player()
{
	EventBuss::Get().RemoveListener(this, EventType::KeyboardEvent);
	EventBuss::Get().RemoveListener(this, EventType::ToggleControlsEvent);
	EventBuss::Get().RemoveListener(this, EventType::MouseMoveAbsoluteEvent);
	EventBuss::Get().RemoveListener(this, EventType::ToggleTetheredEvent);
	EventBuss::Get().RemoveListener(this, EventType::DelegateUpgradeID);
	EventBuss::Get().RemoveListener(this, EventType::GameEventSelectedEvent);
}

bool Player::Initialize(PlayerCamera* camera)
{
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent, EventType::ToggleControlsEvent, EventType::MouseMoveAbsoluteEvent);
	EventBuss::Get().AddListener(this, EventType::ToggleTetheredEvent, EventType::GameEventSelectedEvent);
	EventBuss::Get().AddListener(this, EventType::DelegateUpgradeID);

	m_camera = camera;
	m_ship = new SpaceShip();
	m_topSpeed = m_ship->GetTopSpeed();

	return true;
}

bool Player::update(const std::vector<Planet*>& planets)
{
#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Inventory");
	ImGui::Text("Fuel: %f", m_inventory.fuel);
	ImGui::Text("Oxygen: %f", m_inventory.oxygen);
	ImGui::Text("Titanium: %d", m_inventory.titanium);
	ImGui::Text("Scrap Metal: %d", m_inventory.scrapMetal);
	ImGui::Text("NanoTech: %d", m_inventory.nanotech);
	ImGui::Text("Plasma: %d", m_inventory.plasma);
	ImGui::Text("Radium: %d", m_inventory.radium);
	ImGui::Text("Khionerite: %d", m_inventory.khionerite);
	ImGui::Text("Science: %d", m_inventory.science);
	ImGui::Text("Storage Usage %d/%d", m_storageUsage, m_storageCapacity);
	ImGui::End();
#endif

	DirectX::XMFLOAT3 shipForce = { 0.0f, 0.0f, 0.0f };

	// Handle player input
	if (m_playerControlsActive) {
		UpdateRotation();

		if (m_moveForwards) {
			if (m_stabilizerActive) {
				m_desiredSpeed += m_thrusterForce / m_ship->GetMass() * (float)m_time.DeltaTime();
				if (m_desiredSpeed > *m_topSpeed) m_desiredSpeed = *m_topSpeed;
			}
			else {
				DirectX::XMFLOAT3 desierdVelocity = m_camera->GetForwardVector() * *m_topSpeed;
				shipForce = shipForce + CalculateNeededForce(desierdVelocity);
			}
		}
		if (m_moveBackwards) {
			if (m_stabilizerActive) {
				m_desiredSpeed -= m_thrusterForce / m_ship->GetMass() * (float)m_time.DeltaTime();
				if (m_desiredSpeed < -*m_topSpeed) m_desiredSpeed = -*m_topSpeed;
			}
			else {
				DirectX::XMFLOAT3 desierdVelocity = -1.0f * m_camera->GetForwardVector() * *m_topSpeed;
				shipForce = shipForce + CalculateNeededForce(desierdVelocity);
			}
		}
		if (m_stopMovement) {
			if (m_stabilizerActive) {
				m_desiredSpeed -= m_thrusterForce / m_ship->GetMass() * (float)m_time.DeltaTime();
				if (m_desiredSpeed < 0.0f) m_desiredSpeed = 0.0f;
			}
			else {
				DirectX::XMFLOAT3 velocity = m_ship->GetVelocity();
				float step = m_thrusterForce * (float)m_time.DeltaTime();
				float speed = length(velocity);

				if (speed != 0.0f) {
					// Create a force in the opposite direction of the ships velocity
					DirectX::XMFLOAT3 breakingForce = -1.0f * velocity;
					breakingForce = normalize(breakingForce) * step;
					shipForce = shipForce + breakingForce;
					// We don't want the ship to go backwards, check if to much backwards trust is used
					if (dot(velocity, breakingForce) > 0.0f) {
						shipForce = velocity * m_ship->GetMass();
					}
				}
			}
		}
	}

	//Calculate closest distance to player:
	DetermineClosestPlanet(planets);
	DelegatePlayerInfo();

	if (m_TetheredToClosestPlanet)
	{
		//m_ship->AddForce(m_ship->GetVelocity() * m_ship->GetMass() * -1);
		m_ship->NullifyForces();
		m_ship->SetVelocity(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_PreviousCenterPosition = m_ship->GetCenter();
		m_ship->SetCenter(m_PlayerInfo.planetInteractedWith->GetCenter() - m_TetheredDistanceToUphold);
	}
	else
	{
		if (m_stabilizerActive) {
			DirectX::XMFLOAT3 desierdVelocity = m_camera->GetForwardVector() * m_desiredSpeed;
			shipForce = shipForce + CalculateNeededForce(desierdVelocity);
		}

		shipForce = ConsumeFuel(shipForce);
		ConsumeOxygen();

		m_ship->AddForce(shipForce);
		m_ship->UpdatePhysics();
	}

	DirectX::XMFLOAT3 a = m_ship->getCenter();
	DirectX::XMFLOAT4 shipCenter = { a.x, a.y, a.z, 1.0f };
	m_camera->update(DirectX::XMLoadFloat4(&shipCenter));

	if (length(m_ship->GetVelocity()) < 200.0f) return false;
	return true;
}

SpaceShip* Player::getShip() {
	return this->m_ship;
}

void Player::AddResources(Resources resources)
{
	UpdateHealth(resources.health);

	m_inventory.fuel += resources.fuel;
	if (m_inventory.fuel > m_fuelCapacity) {
		m_inventory.fuel = m_fuelCapacity;
	}
	else if (m_inventory.fuel < 0.0f) {
		m_inventory.fuel = 0.0f;
	}

	m_inventory.oxygen += resources.oxygen;
	if (m_inventory.oxygen > m_oxygenCapacity) {
		m_inventory.oxygen = m_oxygenCapacity;
	}
	else if (m_inventory.oxygen < 0.0f) {
		m_inventory.oxygen = 0.0f;
	}

	m_inventory.plasma += AddToInventory(m_inventory.plasma, resources.plasma);
	m_inventory.radium += AddToInventory(m_inventory.radium, resources.radium);
	m_inventory.khionerite += AddToInventory(m_inventory.khionerite, resources.khionerite);
	m_inventory.nanotech += AddToInventory(m_inventory.nanotech, resources.nanotech);
	m_inventory.titanium += AddToInventory(m_inventory.titanium, resources.titanium);
	m_inventory.scrapMetal += AddToInventory(m_inventory.scrapMetal, resources.scrapMetal);

	m_inventory.science += resources.science;
	if (m_inventory.science < 0) {
		m_inventory.science = 0;
	}
}

void Player::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
		case EventType::MouseMoveAbsoluteEvent:
		{
			float xCoord = static_cast<MouseMoveAbsoluteEvent*>(&event)->GetXCoord();
			float yCoord = static_cast<MouseMoveAbsoluteEvent*>(&event)->GetYCoord();
			m_mousePosX = xCoord;
			m_mousePosY = yCoord;
			break;
		}
		case EventType::KeyboardEvent:
		{
			KeyState state = static_cast<KeyboardEvent*>(&event)->GetKeyState();
			int virKey = static_cast<KeyboardEvent*>(&event)->GetVirtualKeyCode();

			if (state == KeyState::KeyPress && m_playerControlsActive) {
				if (virKey == 'W') {
					PlaySoundEvent thrusterSound(SoundID::Thrusters, true, 1.2f);
					EventBuss::Get().Delegate(thrusterSound);
					m_moveForwards = true;
				}
				if (virKey == 'S') {
					PlaySoundEvent thrusterSound(SoundID::Thrusters, true);
					EventBuss::Get().Delegate(thrusterSound);
					m_moveBackwards = true;
				}
				if (virKey == VK_SPACE) {
					PlaySoundEvent thrusterSound(SoundID::Thrusters, true);
					EventBuss::Get().Delegate(thrusterSound);
					m_stopMovement = true;
				}
				if (virKey == 'Q') {
					if (m_stabilizerActive) m_stabilizerActive = false;
					else m_stabilizerActive = true;
				}
			}
			if (state == KeyState::KeyRelease && m_playerControlsActive) {
				if (virKey == 'W') {
					StopLoopingSoundEvent thrusterSound(SoundID::Thrusters);
					EventBuss::Get().Delegate(thrusterSound);
					m_moveForwards = false;
				}
				if (virKey == 'S') {
					StopLoopingSoundEvent thrusterSound(SoundID::Thrusters);
					EventBuss::Get().Delegate(thrusterSound);
					m_moveBackwards = false;
				}
				if (virKey == VK_SPACE) {
					StopLoopingSoundEvent thrusterSound(SoundID::Thrusters);
					EventBuss::Get().Delegate(thrusterSound);
					m_stopMovement = false;
				}
			}
			break;
		}
		case EventType::GameEventSelectedEvent:
		{
			GameEvent gameEvent = static_cast<GameEventSelectedEvent*>(&event)->GetGameEvent();
			AddResources(gameEvent.reward);
			break;
		}
		case EventType::ToggleControlsEvent:
		{
			if (m_playerControlsActive) m_playerControlsActive = false;
			else m_playerControlsActive = true;
			break;
		}
		case EventType::ToggleTetheredEvent:
		{
			if (m_TetheredToClosestPlanet == false)
			{
				//Get the "distance" that must remain during the whole planet interaction:
				m_PlayerInfo.planetInteractedWith = m_PlayerInfo.closestPlanet;
				m_TetheredDistanceToUphold = m_PlayerInfo.planetInteractedWith->GetCenter() - m_ship->getCenter();
				m_ship->AddForce(m_ship->GetVelocity() * m_ship->GetMass() * -1);
			}
			else
			{
				//Nullify all forces, continue from clean slate:
				m_ship->NullifyForces();
				DirectX::XMFLOAT3 prevCenterToCurCenter = (m_ship->GetCenter() - m_PreviousCenterPosition) * (1 / static_cast<float>(m_time.DeltaTime()));
				m_ship->SetVelocity(prevCenterToCurCenter);
			}
			m_TetheredToClosestPlanet = !m_TetheredToClosestPlanet;
			break;
		}
		case EventType::DelegateUpgradeID:
		{
			unsigned int upgrade = static_cast<DelegateUpgradeID*>(&event)->GetID();
			this->AddResources(static_cast<DelegateUpgradeID*>(&event)->GetCost());
			switch (upgrade)
			{
			case SpaceShip::afterburner: 
			{
				m_ship->Activate(upgrade);
				m_ship->UpgradeToAfterburner();
				break;
			}
			case SpaceShip::antenna:
			{
				m_ship->Activate(upgrade);
				m_HasAntennaUpgrade = true;
				break;
			}
			case SpaceShip::cargo:
			{
				m_ship->Activate(upgrade);
				m_storageCapacity *= 2;
				break;
			}
			case SpaceShip::cold:
			{
				m_ship->Activate(upgrade);
				break;
			}
			case SpaceShip::fuelcells:
			{
				m_ship->Activate(upgrade);
				m_fuelCapacity *= 2.0f;
				m_inventory.fuel += 100.0f;
				break;
			}
			case SpaceShip::livingquarters:
			{
				m_ship->Activate(upgrade);
				m_oxygenCapacity *= 2.0f;
				m_inventory.oxygen += 100.0f;
				break;
			}
			case SpaceShip::hot:
			{
				m_ship->Activate(upgrade);
				break;
			}
			case SpaceShip::radProtect:
			{
				m_ship->Activate(upgrade);
				break;
			}
			case SpaceShip::shield:
			{
				m_ship->Activate(upgrade);
				m_HasShieldUpgrade = true;
				m_maxHealth *= 2;
				m_inventory.health += 100;
				break;
			}
			case SpaceShip::warpdrive:
			{
				m_ship->Activate(upgrade);
				break;
			}
			default:
				break;
			}
			break;
		}
	}
}

void Player::DelegatePlayerInfo() noexcept
{
	m_PlayerInfo.fuelPercentage = m_inventory.fuel / m_fuelCapacity;
	m_PlayerInfo.oxygenPercentage = m_inventory.oxygen / m_oxygenCapacity;
	m_PlayerInfo.HealthPercentage = static_cast<float>(m_inventory.health) / static_cast<float>(m_maxHealth);
	m_PlayerInfo.storageUsage = m_storageUsage;
	m_PlayerInfo.storageCapacity = m_storageCapacity;
	m_PlayerInfo.inventory = m_inventory;
	DelegatePlayerInfoEvent piEvent(&m_PlayerInfo);
	EventBuss::Get().Delegate(piEvent);
}

void Player::UpdateHealth(int value)
{
	m_inventory.health += value;
	if (m_inventory.health > m_maxHealth) {
		m_inventory.health = m_maxHealth;
	}
	else if (m_inventory.health < 0) {
		m_inventory.health = 0;
	}
}

int Player::GetHealth() noexcept {
	return m_inventory.health;
}

int Player::GetMaxHealth() noexcept {
	return m_maxHealth;
}

void Player::UpdateMaxHealth(int value) {
	m_maxHealth += value;

	if (m_inventory.health > m_maxHealth) {
		m_inventory.health = m_maxHealth;
	}
}

void Player::DetermineClosestPlanet(const std::vector<Planet*>& planets) noexcept
{
	Planet* contender = nullptr;
	float closestDistance = D3D11_FLOAT32_MAX;
	for (unsigned int i{ 0u }; i < planets.size(); ++i)
	{
		DirectX::XMVECTOR distance = DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&planets[i]->GetCenter()), DirectX::XMLoadFloat3(&m_ship->GetCenter())));
		if (DirectX::XMVectorGetX(distance) < closestDistance)
		{
			closestDistance = DirectX::XMVectorGetX(distance);
			contender = planets[i];
		}
	}
	m_PlayerInfo.closestPlanet = contender;
	m_PlayerInfo.distanceToClosestPlanet = closestDistance;
}

void Player::Kill() noexcept
{
	m_inventory.health = 0;
}

const bool& Player::HasShieldUpgrade() const noexcept
{
	return m_HasShieldUpgrade;
}

const bool& Player::HasAntennaUpgrade() const noexcept
{
	return m_HasAntennaUpgrade;
}
