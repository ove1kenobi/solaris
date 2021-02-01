#pragma once
#include <DirectXMath.h>
#include <vector>
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

	std::vector<float> m_vertexBuffer;
	std::vector<int> m_indexBuffer;

public:
	GameObject() noexcept;
	~GameObject() = default;

	virtual bool update() = 0;

	void getWMatrix(DirectX::XMMATRIX& wMatrix);
	std::vector<float> getVertexBuffer();
	std::vector<int> getIndexBuffer();
};

