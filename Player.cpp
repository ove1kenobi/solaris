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
	DirectX::XMFLOAT3 velocityVec = m_ship->GetVelocity();
	DirectX::XMFLOAT3 desierdVec = m_camera->GetForwardVector();
	desierdVec.x *= m_desiredSpeed;
	desierdVec.y *= m_desiredSpeed;
	desierdVec.z *= m_desiredSpeed;

	DirectX::XMFLOAT3 stabilizingForce = { desierdVec.x - velocityVec.x,  desierdVec.y - velocityVec.y, desierdVec.z - velocityVec.z };

	if (length(stabilizingForce) > m_thrusterForce * (float)m_time.DeltaTime()) {
		stabilizingForce = normalize(stabilizingForce);
		stabilizingForce.x *= m_thrusterForce * (float)m_time.DeltaTime();
		stabilizingForce.y *= m_thrusterForce * (float)m_time.DeltaTime();
		stabilizingForce.z *= m_thrusterForce * (float)m_time.DeltaTime();
	}

	return stabilizingForce;
}

Player::Player()
{
	m_moveForwards = false;
	m_moveBackwards = false;
	m_stopMovement = false;
	m_playerControlsActive = true;
	m_stabilizerActive = false;
	m_mousePosX = 0.0f;
	m_mousePosY = 0.0f;

	m_ship = nullptr;
	m_camera = nullptr;
	m_rotationSpeed = 1.0f;

	m_topSpeed = 3000.0f;
	m_desiredSpeed = 0.0f;
	m_thrusterForce = 1000.0f;
}

Player::~Player()
{
	
}

bool Player::Initialize(PlayerCamera* camera)
{
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent, EventType::ToggleImGuiEvent, EventType::MouseMoveAbsoluteEvent);

	m_camera = camera;
	m_ship = new SpaceShip();

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
				m_desiredSpeed += m_thrusterForce * m_time.DeltaTime();
				if (m_desiredSpeed > m_topSpeed) m_desiredSpeed = m_topSpeed;
			}
			else {
				float step = m_thrusterForce * (float)m_time.DeltaTime();
				DirectX::XMFLOAT3 direction = m_camera->GetForwardVector();
				shipForce.x += step * direction.x;
				shipForce.y += step * direction.y;
				shipForce.z += step * direction.z;
			}
		}
		if (m_moveBackwards) {
			if (m_stabilizerActive) {
				m_desiredSpeed -= m_thrusterForce * m_time.DeltaTime();
				if (m_desiredSpeed < -m_topSpeed) m_desiredSpeed = -m_topSpeed;
			}
			else {
				float step = -1.0f * m_thrusterForce * (float)m_time.DeltaTime();
				DirectX::XMFLOAT3 direction = m_camera->GetForwardVector();
				shipForce.x += step * direction.x;
				shipForce.y += step * direction.y;
				shipForce.z += step * direction.z;
			}
		}
		if (m_stopMovement) {
			if (m_stabilizerActive) {
				m_desiredSpeed -= m_thrusterForce * m_time.DeltaTime();
				if (m_desiredSpeed < 0.0f) m_desiredSpeed = 0.0f;
			}
			else {
				DirectX::XMFLOAT3 velocity = m_ship->GetVelocity();
				float step = m_thrusterForce * (float)m_time.DeltaTime();
				float speed = length(velocity);

				if (speed != 0.0f) {
					DirectX::XMFLOAT3 breakingForce{ -velocity.x, -velocity.y, -velocity.z };
					breakingForce = normalize(breakingForce);
					breakingForce.x *= step;
					breakingForce.y *= step;
					breakingForce.z *= step;
					shipForce.x += breakingForce.x;
					shipForce.y += breakingForce.y;
					shipForce.z += breakingForce.z;

					if (dot(velocity, breakingForce) > 0.0f) {
						shipForce.x = velocity.x;
						shipForce.y = velocity.y;
						shipForce.z = velocity.z;
					}
				}
			}
		}
	}

	if (m_stabilizerActive) {
		DirectX::XMFLOAT3 stabilizingForce = Stabilize();
		shipForce.x += stabilizingForce.x;
		shipForce.y += stabilizingForce.y;
		shipForce.z += stabilizingForce.z;
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
					m_moveForwards = true;
				}
				if (virKey == 'S') {
					m_moveBackwards = true;
				}
				if (virKey == VK_SPACE) {
					m_stopMovement = true;
				}
				if (virKey == 'Q') {
					if (m_stabilizerActive) m_stabilizerActive = false;
					else m_stabilizerActive = true;
				}
			}

			if (state == KeyState::KeyRelease) {
				if (virKey == 'W') {
					m_moveForwards = false;
				}
				if (virKey == 'S') {
					m_moveBackwards = false;
				}
				if (virKey == VK_SPACE) {
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
