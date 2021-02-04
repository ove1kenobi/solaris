#pragma once
#include <string>

#include "Camera.h"
#include "EventSystem\IEventListener.h"
#include "EventSystem/EventBuss.h"
#include "EventSystem/MouseMoveEvent.h"
#include "EventSystem/MouseScrollEvent.h"
#include "DirectXTK/Mouse.h"

class PlayerCamera : public IEventListener, public Camera {
private:
	float m_FOVvalue = 4.0f;
	float m_distanceFromShip;
	float m_sensitivity;
	void OnEvent(IEvent& event) noexcept;

public:
	PlayerCamera() = default;
	~PlayerCamera() = default;

	//Initializes the pMatrix
	bool init(int screenWidth, int screenHeight);
	//Updates the vMatrix
	void update(DirectX::XMVECTOR shipCoords);
	//Move the camera the same amount that the spaceship moved.
	void move(DirectX::XMVECTOR shipCoordsDiff);
	//Mouse movement
	void mouseRot();
	//Zoom
	void mouseScroll();
	//Rotates camera when ship rotates.
	void shipRot(float step);
};