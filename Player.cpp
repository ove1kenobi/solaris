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
{
	m_fuelCapacity = 500;
	m_oxygenCapacity = 500;
	m_storageCapacity = 1000;
	for (unsigned int i = 0; i < numberOfResources; i++) {
		m_resources[i] = 0;
	}
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
}

Player::~Player()
{
	
}

bool Player::Initialize(PlayerCamera* camera)
{
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent, EventType::ToggleImGuiEvent, EventType::MouseMoveAbsoluteEvent);

	m_camera = camera;
	m_ship = new SpaceShip();
	m_topSpeed = m_ship->GetTopSpeed();
	/*
	std::cout << "Test 1: " << std::endl;
	AddResource(100, Resource::Fuel);
	AddResource(450, Resource::Fuel);
	AddResource(100, Resource::Fuel);
	AddResource(-200, Resource::Fuel);

	std::cout << "Test 2: " << std::endl;
	AddResource(-600, Resource::Oxygen);
	AddResource(-100, Resource::Oxygen);
	AddResource(1, Resource::Oxygen);
	AddResource(0, Resource::Oxygen);

	std::cout << "Test 3: " << std::endl;
	AddResource(400, Resource::Titanium);
	AddResource(300, Resource::Khionerite);
	AddResource(1000, Resource::Nanotech);
	AddResource(100, Resource::Nanotech);
	AddResource(-500, Resource::Plasma);
	AddResource(-200, Resource::Titanium);
	AddResource(-1000, Resource::Nanotech);*/


	return true;
}

bool Player::update()
{
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

	return false;
}

SpaceShip* Player::getShip() {
	return this->m_ship;
}

void Player::AddResource(int amount, Resource resource)
{
	int index = static_cast<int>(resource);

	switch (resource)
	{
		case Resource::Fuel:
		{
			m_resources[index] += amount;
			if (m_resources[index] > m_fuelCapacity) {
				m_resources[index] = m_fuelCapacity;
			}
			break;
		}
		case Resource::Oxygen:
		{
			m_resources[index] += amount;
			if (m_resources[index] > m_oxygenCapacity) {
				m_resources[index] = m_oxygenCapacity;
			}
			break;
		}
		default:
		{
			int newUsage = m_storageUsage + amount;
			if (newUsage > m_storageCapacity) {
				m_resources[index] += m_storageCapacity - m_storageUsage;
				m_storageUsage = m_storageCapacity;
			}
			else {
				m_resources[index] += amount;
				m_storageUsage += amount;
			}
			break;
		}
	}

	if (m_resources[index] < 0) {
		m_resources[index] = 0;
	}
	if (m_storageUsage < 0) {
		m_storageUsage = 0;
	}

	//for (unsigned int i = 0; i < numberOfResources; i++) {
	//	std::cout << m_resources[i] << std::endl;
	//}
	//std::cout << std::endl << m_storageUsage << std::endl << std::endl << std::endl;
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
		case EventType::ToggleImGuiEvent:
		{
			if (m_playerControlsActive) m_playerControlsActive = false;
			else m_playerControlsActive = true;
			break;
		}
	}
}
