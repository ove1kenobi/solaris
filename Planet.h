#pragma once
#include <DirectXMath.h>

#include <cmath>
#include <vector>

#include "ourMath.h"
class Planet
{
private:
	//Called in init to create the sphere.
	void createSphere();
	//Called in createSphere to create a new triangle.
	void createTriangleFace(std::vector<int> edge1, std::vector<int> edge2, std::vector<int> edge3, bool reverse, std::vector<DirectX::XMFLOAT3> &vertices, std::vector<int>& triangles);

	std::vector<DirectX::XMFLOAT3> m_vertexBuffer;
	std::vector<int> m_indexBuffer;

	DirectX::XMFLOAT3 m_center;
	float m_radius;
	float m_mass;

	float m_pitch;
	float m_yaw;
public:
	Planet();
	~Planet();

	bool init(float x, float y, float z, float r);
};

