#pragma once
#include <DirectXMath.h>

#include <cmath>

#include "GameObject.h"
#include "ourMath.h"

//Represents a planet, moon or asteroid.
class CosmicBody : public GameObject
{
private:
	/*
	//Called in init to create the sphere.
	void createSphere();
	//Called in createSphere to create a new triangle.
	void createTriangleFace(std::vector<int> edge1, std::vector<int> edge2, std::vector<int> edge3, bool reverse, std::vector<DirectX::XMFLOAT3> &vertices, std::vector<int>& triangles);
	*/
	float m_radius;

	unsigned int m_divisions;
	
public:
	CosmicBody() noexcept;
	~CosmicBody() = default;

	bool init(float x, float y, float z, float r);
	bool update();
};

