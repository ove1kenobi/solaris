#pragma once
#include "Camera.h"
#include "EventSystem\IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem\UtilityEvents.h"

class PlayerCamera : public IEventListener, public EventPublisher, public Camera {
private:
	float m_FOVvalue = 4.0f;
	float m_distanceFromShip;
	float m_sensitivity;
	bool m_orbitModeActive;
	void OnEvent(IEvent& event) noexcept;

public:
	PlayerCamera();
	~PlayerCamera() = default;

	//Initializes the pMatrix
	bool init(int screenWidth, int screenHeight);
	//Updates the vMatrix
	void update(DirectX::XMVECTOR shipCoords);
	//Mouse movement
	void mouseRot(int xValue, int yValue);
	//Zoom
	void mouseScroll(int scroll);
	//Rotates camera when ship rotates.
	void shipRot(float step);

	DirectX::XMVECTOR GetForward();
};