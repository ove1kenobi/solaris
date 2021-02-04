#pragma once
#include <DirectXMath.h>
#include "CosmicBody.h"
#include "PointLight.h"

class Sun : public CosmicBody {
private:
	PointLight light;
public:
	Sun();
	~Sun();
	bool init();
};

/*What does the sun need?
* - GameObject: mass
* - GameObject: centre point
* - GameObject: world matrix
* - CosmicBody: radius
* - ModelFactory: vertex buffer (if generated)
* - ModelFactory: index buffer (if generated)
* 
* What does the point light need?
* - position (GameObject)
* - range
* - attenuation
*/
