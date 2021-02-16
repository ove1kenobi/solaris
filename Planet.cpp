#include "pch.h"
#include "Planet.h"

bool Planet::Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo, Orbit* orbit) {
	//The cosmic body
	this->init(x, y, z, r, xRot, zRot, rotDir, tetherTo, orbit);
	
	//Generate the Planet.
	this->m_model = ModelFactory::Get().GeneratePlanet(x, y, z, r);
	return true;
}