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
* - mass
* - radius
* - centre point
* - world matrix
* - vertex buffer (if generated)
* - index buffer (if generated)
* 
* What does the point light need?
* - position
* - range
* - attenuation
*/
