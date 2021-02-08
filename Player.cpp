#include "pch.h"
#include "Player.h"

Player::Player()
{
	m_moveForwards = false;
	m_moveBackwards = false;
	m_moveUp = false;
	m_moveDown = false;
	m_rotateRight = false;
	m_rotateLeft = false;

	m_ship = nullptr;
	m_camera = nullptr;
	m_speed = 100.0f;
	m_rotation = (float)M_PI_4;

	m_forwardVector = { 0.0f, 0.0f, 1.0f };
	m_rightVector = { 1.0f, 0.0f, 0.0f };
	m_upVector = {0.0f, 1.0f, 0.0f};
}

Player::~Player()
{
}

bool Player::Initialize(/*DirectX::XMFLOAT3 position,*/ PlayerCamera* camera)
{
	EventBuss::Get().AddListener(this, EventType::MouseMoveEvent);
	EventBuss::Get().AddListener(this, EventType::MouseButtenEvent);
	EventBuss::Get().AddListener(this, EventType::KeyboardEvent);

	m_ship = new SpaceShip();
	m_camera = camera;
	//m_center = position;
	//m_upVector = {0.0f, 1.0f, 0.0f};

	return true;
}

void Player::Move(DirectX::XMFLOAT3 direction)
{
	float step = m_speed * (float)m_time.DeltaTime();
	DirectX::XMFLOAT4 offset(direction.x * step, direction.y * step, direction.z * step, 0.0f);

	//m_center.x += offset.x;
	//m_center.y += offset.y;
	//m_center.z += offset.z;
	m_ship->move(offset);
	
	
	
	//DirectX::XMVECTOR offsetVec = DirectX::XMLoadFloat4(&offset);
}

void Player::YawRotation(float rotation)
{
	float step = rotation * (float)m_time.DeltaTime();

	// Rotating right and forward vector in a 2D plane
	m_rightVector.x = cos(step)*m_rightVector.x - sin(step)*m_rightVector.z;
	m_rightVector.z = sin(step)*m_rightVector.x + cos(step)*m_rightVector.z;

	m_forwardVector.x = cos(step)*m_forwardVector.x - sin(step)*m_forwardVector.z;
	m_forwardVector.z = sin(step)*m_forwardVector.x + cos(step)*m_forwardVector.z;

	// rotate model
	this->m_ship->rotate(step);

	//rotate camera
	this->m_camera->shipRot(step);
}

bool Player::update()
{
	if (m_moveForwards) {
		Move(m_forwardVector);
	}
	else if (m_moveBackwards) {
		DirectX::XMFLOAT3 backVector = {-m_forwardVector.x, -m_forwardVector.y, -m_forwardVector.z };
		Move(backVector);
	}

	if (m_rotateRight) {
		YawRotation(-m_rotation);
	} 
	else if (m_rotateLeft) {
		YawRotation(m_rotation);
	}

	if (m_moveUp) {
		Move(m_upVector);
	}
	else if (m_moveDown) {
		DirectX::XMFLOAT3 downVector = { -m_upVector.x, -m_upVector.y, -m_upVector.z };
		Move(downVector);
	}

	DirectX::XMFLOAT3 a = m_ship->getCenter();
	DirectX::XMFLOAT4 shipCenter = { a.x, a.y, a.z, 1.0f };
	m_camera->update(DirectX::XMLoadFloat4(&shipCenter));

	//std::cout << "position: \t" << m_center.x << '\t' << m_center.y << '\t' << m_center.z << std::endl;

	//DirectX::XMVECTOR positionVec;
	//DirectX::XMStoreFloat3(&m_center, positionVec);
	//m_camera->update(positionVec);

	return false;
}

SpaceShip* Player::getShip() {
	return this->m_ship;
}

void Player::OnEvent(IEvent& event) noexcept
{
	//std::cout << "Event recived" << std::endl;
	switch (event.GetEventType())
	{
		/*
		case EventType::MouseMoveEvent:
		{
			int xCoord = static_cast<MouseMoveEvent*>(&event)->GetXCoord();
			int yCoord = static_cast<MouseMoveEvent*>(&event)->GetYCoord();

			//std::cout << "Mouse moved to " << xCoord << " " << yCoord << std::endl;
			break;
		}
		*/
		case EventType::MouseButtenEvent:
		{
			KeyState state = static_cast<MouseButtenEvent*>(&event)->GetKeyState();
			int virKey = static_cast<MouseButtenEvent*>(&event)->GetVirtualKeyCode();
			int xCoord = static_cast<MouseButtenEvent*>(&event)->GetXCoord();
			int yCoord = static_cast<MouseButtenEvent*>(&event)->GetYCoord();

			/*
			std::cout << "Mouse butten " << virKey << " was ";
			if (state == KeyState::KeyPress)
				std::cout << "pressed";
			else if (state == KeyState::KeyRelease)
				std::cout << "releassed";
			
			std::cout << " at " << xCoord << " " << yCoord << std::endl;
			*/
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
				else if (virKey == 'S') {
					m_moveBackwards = true;
				}

				if (virKey == 'D') {
					m_rotateRight = true;
				}
				else if (virKey == 'A') {
					m_rotateLeft = true;
				}

				if (virKey == VK_SPACE) {
					m_moveUp = true;
				}
				else if (virKey == VK_SHIFT) {
					m_moveDown = true;
				}
			}

			if (state == KeyState::KeyRelease) {
				if (virKey == 'W') {
					m_moveForwards = false;
				}
				else if (virKey == 'S') {
					m_moveBackwards = false;
				}

				if (virKey == 'D') {
					m_rotateRight = false;
				}
				else if (virKey == 'A') {
					m_rotateLeft = false;
				}

				if (virKey == VK_SPACE) {
					m_moveUp = false;
				}
				else if (virKey == VK_SHIFT) {
					m_moveDown = false;
				}
			}
			
			break;
		}
	}
}
