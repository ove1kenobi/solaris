#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <cmath>

class Player
{
private:
	// Model
	// PlayerCamera* camera;
	// Timer 
	float deltaTime;

	DirectX::XMFLOAT3 m_forwardVector;
	DirectX::XMFLOAT3 m_rightVector;
	DirectX::XMFLOAT3 m_upVector;
	DirectX::XMFLOAT3 m_pos;

	float m_speed, m_roation;
	float m_pitch, m_roll, m_yaw;

	void Move(DirectX::XMFLOAT3 direction);
	void YawRotation();

public:
	Player();
	~Player();

	void Init(DirectX::XMFLOAT3 position);
};

