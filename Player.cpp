#include "Player.h"



Player::Player()
{ 
}

Player::~Player()
{
}



void Player::Init(DirectX::XMFLOAT3 position)
{
	m_pos = position;
	m_upVector.x = 0.0f;
	m_upVector.y = 1.0f;
	m_upVector.z = 0.0f;
	// m_forwardVector = Calculate forward vector based on rotation and normalize it 
	// m_rightVector = m_upVecto x m_fowardVector, take cross product between up and forward vector and then normalize
}

void Player::Move(DirectX::XMFLOAT3 direction)
{
	float step = m_speed * deltaTime;
	DirectX::XMFLOAT3 offset(direction.x * step, direction.y * step, direction.z * step);

	m_pos.x += offset.x;
	m_pos.y += offset.y;
	m_pos.z += offset.z;

	// move camera and model with offset?
}

void Player::YawRotation()
{
	float step = m_roation * deltaTime;

	m_rightVector.y = cos(step)*m_rightVector.y - sin(step)*m_rightVector.z;
	m_rightVector.z = sin(step)*m_rightVector.y + cos(step)*m_rightVector.z;

	m_forwardVector.y = cos(step)*m_forwardVector.y - sin(step)*m_forwardVector.z;
	m_forwardVector.z = sin(step)*m_forwardVector.y + cos(step)*m_forwardVector.z;

	// rotate model
}
