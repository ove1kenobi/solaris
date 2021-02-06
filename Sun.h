#pragma once
#include "CosmicBody.h"
#include "PointLight.h"

//Sun is of the type CosmicBody, as it needs the same properties
class Sun : public CosmicBody {
private:
	//But it also contains a light source
	PointLight light;
public:
	Sun() = default;
	~Sun() = default;
	//Uses the CosmicBody init() and light init() to set everything up related to the sun
	bool Initialize();
};
