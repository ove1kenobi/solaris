#include "pch.h"
#include "Player.h"

void Player::UpdateRotation()
{
	float yaw = m_mousePosX * m_time.DeltaTime();
	float pitch = -m_mousePosY * m_time.DeltaTime();
	float roll = -m_mousePosX * m_time.DeltaTime();
	m_camera->OrbitRotation(yaw, pitch);
	m_ship->AddRotation(yaw, pitch);
	m_ship->SetTilt(-m_mousePosY, -m_mousePosX);

	m_ship->SetForwardVector(m_camera->getPos());
}

Player::Player()
{
	m_moveForwards = false;
	m_moveBackwards = false;
	m_adjustRotation = false;
	m_mousePosX = 0.0f;
	m_mousePosY = 0.0f;

	m_ship = nullptr;
	m_camera = nullptr;
	m_speed = 1000.0f;
}

Player::~Player()
{
	
}

bool Player::Initialize(/*DirectX::XMFLOAT3 position,*/ PlayerCamera* camera)
{
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent, EventType::MouseMoveAbsoluteEvent);

	m_camera = camera;
	m_ship = new SpaceShip();
	m_adjustRotation = true;

	return true;
}

bool Player::update()
{
	DirectX::XMFLOAT3 a = m_ship->getCenter();
	DirectX::XMFLOAT4 shipCenter = { a.x, a.y, a.z, 1.0f };
	m_camera->update(DirectX::XMLoadFloat4(&shipCenter));

	UpdateRotation();

	if (m_moveForwards) {
		m_ship->Move(m_speed * m_time.DeltaTime());
	}
	else if (m_moveBackwards) {
		m_ship->Move(-1 * m_speed * m_time.DeltaTime());
	}

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

			/*
			if (-0.1f < xCoord && xCoord < 0.1f) {
				m_mousePosX = 0.0f;
			}
			else {
				m_mousePosX = xCoord;
			}

			if (-0.1f < yCoord && yCoord < 0.1f) {
				m_mousePosY = 0.0f;
			}
			else {
				m_mousePosY = yCoord;
			}*/
		}

		/*
		case EventType::MouseMoveRelativeEvent:
		{
			int xDiff = static_cast<MouseMoveRelativeEvent*>(&event)->GetXDiff();
			int yDiff = static_cast<MouseMoveRelativeEvent*>(&event)->GetYDiff();
			
			float yaw = xDiff * m_time.DeltaTime() / 0.2f;
			float pitch = -yDiff * m_time.DeltaTime() / 0.2f;
			float roll = -xDiff * m_time.DeltaTime() * 2.0f;
			m_ship->Rotate(yaw, pitch, roll);
			m_ship->SetForwardVector(m_camera->getPos());
			m_adjustRotation = false;
		}*/

		case EventType::KeyboardEvent:
		{
			KeyState state = static_cast<KeyboardEvent*>(&event)->GetKeyState();
			int virKey = static_cast<KeyboardEvent*>(&event)->GetVirtualKeyCode();

			if (state == KeyState::KeyPress) {
				if (virKey == 'W') {
					m_moveForwards = true;
				}
				else if (virKey == 'S') {
					m_moveBackwards = true;
				}
			}

			if (state == KeyState::KeyRelease) {
				if (virKey == 'W') {
					m_moveForwards = false;
				}
				else if (virKey == 'S') {
					m_moveBackwards = false;
				}
			}
			
			break;
		}
	}
}
