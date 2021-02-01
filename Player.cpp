#include "Player.h"



Player::Player()
{
	m_camera = nullptr;
	m_speed = 1.0f;
	m_roation = (float)M_PI_4;

	m_forwardVector = { 0.0f, 0.0f, 1.0f };
	m_rightVector = { 1.0f, 0.0f, 0.0f };
	m_upVector = {0.0f, 1.0f, 0.0f};
}

Player::~Player()
{
}



void Player::init(DirectX::XMFLOAT3 position, PlayerCamera* camera)
{
	m_camera = camera;
	m_center = position;
	//m_upVector = {0.0f, 1.0f, 0.0f};
}

void Player::move(DirectX::XMFLOAT3 direction)
{
	float step = m_speed * (float)m_time.DeltaTime();
	DirectX::XMFLOAT3 offset(direction.x * step, direction.y * step, direction.z * step);

	m_center.x += offset.x;
	m_center.y += offset.y;
	m_center.z += offset.z;

	DirectX::XMVECTOR offsetVec;
	DirectX::XMStoreFloat3(&offset, offsetVec);
	m_camera->move(offsetVec);

	// move model
}

void Player::yawRotation()
{
	float step = m_roation * (float)m_time.DeltaTime();

	// Rotating right and forward vector in a 2D plane
	m_rightVector.y = cos(step)*m_rightVector.y - sin(step)*m_rightVector.z;
	m_rightVector.z = sin(step)*m_rightVector.y + cos(step)*m_rightVector.z;

	m_forwardVector.y = cos(step)*m_forwardVector.y - sin(step)*m_forwardVector.z;
	m_forwardVector.z = sin(step)*m_forwardVector.y + cos(step)*m_forwardVector.z;

	// rotate model
}

bool Player::update()
{
	// check event message for input 
	// if relevent input is recived, move or rotate player, model and camera

	DirectX::XMVECTOR positionVec;
	DirectX::XMStoreFloat3(&m_center, positionVec);
	m_camera->update(positionVec);

	return false;
}