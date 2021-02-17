#pragma once
#include "Camera.h"
#include "EventSystem\IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem\UtilityEvents.h"

class PlayerCamera : public IEventListener, public EventPublisher, public Camera {
private:
	float m_mousePosX, m_mousePosY;
	float m_maxScroll, m_minScroll;
	float m_FOVvalue = 4.0f;
	float m_distanceFromShip;
	void OnEvent(IEvent& event) noexcept;

public:
	PlayerCamera();
	~PlayerCamera() = default;

	//Initializes the pMatrix
	bool init(int screenWidth, int screenHeight);
	//Updates the vMatrix
	void update(DirectX::XMVECTOR shipCoords);
	//Rotate orbit camera movement
	void OrbitRotation(float yaw, float pitch);
	//Zoom
	void mouseScroll(int scroll);
};