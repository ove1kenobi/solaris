#pragma once
#include <string>

#include "Camera.h"

class PlayerCamera : public Camera {
public:
	PlayerCamera() = default;
	~PlayerCamera() = default;

	//Initializes the pMatrix
	bool init(int screenWidth, int screenHeight, float screenNear, float screenFar, float FOVvalue);
	//Updates the vMatrix
	void update(DirectX::XMVECTOR shipCoords);
	//Move the camera the same amount that the spaceship moved.
	void move(DirectX::XMVECTOR shipCoordsDiff);
	//Mouse movement
	void mouseRot(int mouseX, int mouseY);
};