#include "pch.h"
#include "Planet.h"

bool Planet::Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir) {
	//The cosmic body
	this->init(x, y, z, r, xRot, zRot, rotDir);
	
	//Generate the Planet.
	this->m_model = ModelFactory::Get().GeneratePlanet(x, y, z, r);
	return true;
}