#pragma once
#include <DirectXMath.h>

#include <cmath>
#include <vector>
class Planet
{
private:
	bool createSphere();

	std::vector<DirectX::XMFLOAT3> m_vertices;
	std::vector<int> m_indices;

	DirectX::XMFLOAT3 m_center;
	float m_radius;
public:
	Planet();
	~Planet();

	bool init(float x, float y, float z, float r);
};

