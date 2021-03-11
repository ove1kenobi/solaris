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

DirectX::XMFLOAT3 Player::Stabilize()
{
	// Get the current velocity vector
	DirectX::XMFLOAT3 velocity = m_ship->GetVelocity();
	// Get the velocity vector the player wants based on there direction and given speed
	DirectX::XMFLOAT3 desierdVelocity = m_camera->GetForwardVector() * m_desiredSpeed;
	// Calculate how much force would be required to redirect the ships velocity to the desierd velocity
	DirectX::XMFLOAT3 stabilizingForce = (desierdVelocity - velocity) * m_ship->GetMass();
	// Check if the trusters have enouth power to produce that force
	if (length(stabilizingForce) > m_thrusterForce * (float)m_time.DeltaTime()) {
		stabilizingForce = normalize(stabilizingForce);
		stabilizingForce = stabilizingForce * m_thrusterForce * (float)m_time.DeltaTime();
	}

	return stabilizingForce;
}

Player::Player()
	: m_PlayerInfo{ }
{
	m_fuelCapacity = 500;
	m_oxygenCapacity = 500;
	m_storageCapacity = 1000;
	m_inventory.fuel = 0;
	m_inventory.oxygen = 0;
	m_inventory.titanium = 0;
	m_inventory.scrapMetal = 0;
	m_inventory.nanotech = 0;
	m_inventory.plasma = 0;
	m_inventory.radium = 0;
	m_inventory.khionerite = 0;
	m_storageUsage = 0;

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

	m_topSpeed = 0.0f;
	m_desiredSpeed = 0.0f;
	m_thrusterForce = 10000000.0f;

	m_maxHealth = 100;
	m_currentHealth = 100;
}

Player::~Player()
{
	
}

bool Player::Initialize(PlayerCamera* camera)
{
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent, EventType::ToggleImGuiEvent, EventType::MouseMoveAbsoluteEvent);
	EventBuss::Get().AddListener(this, EventType::GameEventSelectedEvent);

	m_camera = camera;
	m_ship = new SpaceShip();
	m_topSpeed = m_ship->GetTopSpeed();

	return true;
}

bool Player::update(const std::vector<Planet*>& planets)
{
#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("Inventiry");
	ImGui::Text("Fuel: %d", m_inventory.fuel);
	ImGui::Text("Oxygen: %d", m_inventory.oxygen);
	ImGui::Text("Titanium: %d", m_inventory.titanium);
	ImGui::Text("Scrap Metal: %d", m_inventory.scrapMetal);
	ImGui::Text("Nanotech: %d", m_inventory.nanotech);
	ImGui::Text("Plasma: %d", m_inventory.plasma);
	ImGui::Text("Radium: %d", m_inventory.radium);
	ImGui::Text("Khionerite: %d", m_inventory.khionerite);
	//ImGui::Text("Storage Usage %d/%d", m_storageUsage, m_storageCapacity);
	ImGui::End();
#endif

	DirectX::XMFLOAT3 shipForce = { 0.0f, 0.0f, 0.0f };

	// Handle player input
	if (m_playerControlsActive) {
		UpdateRotation();

		if (m_moveForwards) {
			if (m_stabilizerActive) {
				m_desiredSpeed += m_thrusterForce / m_ship->GetMass() * (float)m_time.DeltaTime();
				if (m_desiredSpeed > m_topSpeed) m_desiredSpeed = m_topSpeed;
			}
			else {
				float step = m_thrusterForce * (float)m_time.DeltaTime();
				DirectX::XMFLOAT3 direction = m_camera->GetForwardVector();
				shipForce = shipForce + direction * step;
			}
		}
		if (m_moveBackwards) {
			if (m_stabilizerActive) {
				m_desiredSpeed -= m_thrusterForce / m_ship->GetMass() * (float)m_time.DeltaTime();
				if (m_desiredSpeed < -m_topSpeed) m_desiredSpeed = -m_topSpeed;
			}
			else {
				float step = -1.0f * m_thrusterForce * (float)m_time.DeltaTime();
				DirectX::XMFLOAT3 direction = m_camera->GetForwardVector();
				shipForce = shipForce + direction * step;
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

	if (m_stabilizerActive) {
		DirectX::XMFLOAT3 stabilizingForce = Stabilize();
		shipForce = shipForce + stabilizingForce;
	}
	m_ship->AddForce(shipForce);
	m_ship->UpdatePhysics();

	DirectX::XMFLOAT3 a = m_ship->getCenter();
	DirectX::XMFLOAT4 shipCenter = { a.x, a.y, a.z, 1.0f };
	m_camera->update(DirectX::XMLoadFloat4(&shipCenter));

	//Calculate closest distance to player:
	DetermineClosestPlanet(planets);
	DelegatePlayerInfo();

	if (length(m_ship->GetVelocity()) < 500.0f) return false;
	return true;
}

SpaceShip* Player::getShip() {
	return this->m_ship;
}

void Player::AddResources(Resources resources)
{
	/*m_inventory.fuel += resources.fuel;
	if (m_inventory.fuel > m_fuelCapacity) {
		m_inventory.fuel = m_fuelCapacity;
	}

	m_inventory.oxygen += resources.oxygen;
	if (m_inventory.oxygen > m_oxygenCapacity) {
		m_inventory.oxygen = m_oxygenCapacity;
	}*/


	m_inventory.fuel += resources.fuel;
	m_inventory.oxygen += resources.oxygen;
	m_inventory.titanium += resources.titanium;
	m_inventory.scrapMetal += resources.scrapMetal;
	m_inventory.nanotech += resources.nanotech;
	m_inventory.plasma += resources.plasma;
	m_inventory.radium += resources.radium;
	m_inventory.khionerite += resources.khionerite;

	//switch (resources)
	//{

	//	case Resource::Oxygen:
	//	{
	//		m_resources[index] += amount;
	//		if (m_resources[index] > m_oxygenCapacity) {
	//			m_resources[index] = m_oxygenCapacity;
	//		}
	//		break;
	//	}
	//	default:
	//	{
	//		int storageLeft = m_storageCapacity - m_storageUsage;

	//		// Storage space have run out, add only as much as you can
	//		if (storageLeft < amount) {
	//			m_resources[index] += storageLeft;
	//			m_storageUsage += storageLeft;
	//		}

	//		// We can't have less then 0 of a resource, only remove what we have
	//		else if (m_resources[index] + amount < 0) {
	//			int tempAmount = m_resources[index];
	//			m_resources[index] = 0;
	//			m_storageUsage -= tempAmount;
	//		}

	//		// Normal scenario
	//		else {
	//			m_resources[index] += amount;
	//			m_storageUsage += amount;
	//		}

	//		break;
	//	}
	//}

	//if (m_resources[index] < 0) {
	//	m_resources[index] = 0;
	//}
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


			/*if (virKey == 'O' && state == KeyState::KeyPress) {
				UINT gameEventID[3];
				gameEvManager.RequestGameEvents(gameEventID, 2);

				for (int i = 0; i < 3; i++) {
					GameEvent gameEv = GetGameEvent(gameEventID[i]);

				    std::cout << gameEv.prologue << std::endl;
				    std::cout << gameEv.consequence << std::endl;
					std::cout << gameEv.reward.fuel << " " << gameEv.reward.oxygen << " " << gameEv.reward.titanium << " "
						<< gameEv.reward.scrapMetal << " " << gameEv.reward.nanotech << " " << gameEv.reward.plasma << " "
						<< gameEv.reward.radium << " " << gameEv.reward.khionerite << std::endl;
				    std::cout << std::endl << std::endl;
				}
			}*/



			if (state == KeyState::KeyPress) {
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

			if (state == KeyState::KeyRelease) {
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
			UINT gameEventID = static_cast<GameEventSelectedEvent*>(&event)->GetGameEventID();
			Resources reward = GetGameEvent(gameEventID).reward;
			AddResources(reward);
			break;
		}
		case EventType::ToggleImGuiEvent:
		{
			if (m_playerControlsActive) m_playerControlsActive = false;
			else m_playerControlsActive = true;
			break;
		}
	}
}

void Player::DelegatePlayerInfo() noexcept
{
	DelegatePlayerInfoEvent piEvent(&m_PlayerInfo);
	EventBuss::Get().Delegate(piEvent);
}

int Player::GetHealth() noexcept {
	return m_currentHealth;
}

void Player::UpdateHealth(int value) {
	m_currentHealth += value;
}

int Player::GetMaxHealth() noexcept {
	return m_maxHealth;
}

void Player::UpdateMaxHealth(int value) {
	m_maxHealth += value;

	if (m_currentHealth > m_maxHealth) {
		m_currentHealth = m_maxHealth;
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
