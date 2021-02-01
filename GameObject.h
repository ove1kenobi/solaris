#pragma once
#include <DirectXMath.h>

#include "Time.h"
class GameObject
{
protected:
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_center;

	DirectX::XMFLOAT4X4 m_wMatrix;

	float m_mass;
	float m_pitch;
	float m_roll;
	float m_yaw;

	Time m_timer;
public:
	GameObject() noexcept;
	~GameObject() = default;

	virtual bool update() = 0;
};

