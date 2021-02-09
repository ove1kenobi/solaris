#include "pch.h"
#include "PlayerCamera.h"

bool PlayerCamera::init(int screenWidth, int screenHeight) {
	//DirectXTK mouse
	EventBuss::Get().AddListener(this, EventType::MouseMoveRelativeEvent, EventType::MouseScrollEvent, EventType::ToggleImGuiEvent);

	this->m_screenFar = 100000.0f;
	float FOV = 3.141592654f / this->m_FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	m_orbitModeActive = true;

	//Initial camera distance from the ship.
	this->m_distanceFromShip = 300.0f;

	this->m_sensitivity = 0.2f;

	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, this->m_screenNear, this->m_screenFar));
	return true;
}

void PlayerCamera::update(DirectX::XMVECTOR shipCoords) {
	//Orbit around the ship.
	this->m_forwardVector = shipCoords;
	
	this->m_posVector = DirectX::XMVectorSetX(this->m_posVector, sinf(this->m_pitch) * sinf(this->m_yaw) * this->m_distanceFromShip);
	this->m_posVector = DirectX::XMVectorSetY(this->m_posVector, cosf(this->m_pitch) * this->m_distanceFromShip);
	this->m_posVector = DirectX::XMVectorSetZ(this->m_posVector, -sinf(this->m_pitch) * cosf(this->m_yaw) * this->m_distanceFromShip);

	this->m_posVector = DirectX::XMVectorAdd(this->m_posVector, this->m_forwardVector);
	DirectX::XMStoreFloat4x4(&this->m_vMatrix, DirectX::XMMatrixLookAtLH(this->m_posVector, this->m_forwardVector, this->m_upVector));
}

void PlayerCamera::mouseRot(int xValue, int yValue) {

	float pi = static_cast<float>((atan(1) * 4));

	this->m_yaw -= xValue * (float)m_time.DeltaTime() / this->m_sensitivity;

	//Limits camera rotation up and down.
	if (this->m_pitch < pi && this->m_pitch > 0.0f)
		this->m_pitch -= yValue * (float)m_time.DeltaTime() / this->m_sensitivity;
	if (this->m_pitch >= pi)
		this->m_pitch = pi - 0.0001f;
	if (this->m_pitch <= 0.0f)
		this->m_pitch = 0.0001f;
}

void PlayerCamera::mouseScroll(int scroll) {
	//Limits scroll between 600 to 50 atm. TODO: Make variables.
	if(this->m_distanceFromShip < 600.0f && this->m_distanceFromShip > 50.0f)
		this->m_distanceFromShip -= scroll  / 3;
	if (this->m_distanceFromShip >= 600.0f)
		this->m_distanceFromShip = 600.0f - 0.1f;
	if (this->m_distanceFromShip <= 50.0f)
		this->m_distanceFromShip = 50.0f + 0.1f;
}

//How much the camera rotates when the ship rotates
void PlayerCamera::shipRot(float step) {
	this->m_yaw +=  step/* * 3 / 4*/;
}

void PlayerCamera::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		case EventType::MouseMoveRelativeEvent:
		{
			int xDiff = static_cast<MouseMoveRelativeEvent*>(&event)->GetXDiff();
			int yDiff = static_cast<MouseMoveRelativeEvent*>(&event)->GetYDiff();
			if (m_orbitModeActive)
			{
				mouseRot(xDiff, yDiff);
			}
			break;
		}
		case EventType::MouseScrollEvent:
		{
			int wheelScroll = static_cast<MouseScrollEvent*>(&event)->GetWheelScroll();
			mouseScroll(wheelScroll);
			break;
		}
		case EventType::ToggleImGuiEvent:
		{
			if (m_orbitModeActive) m_orbitModeActive = false;
			else m_orbitModeActive = true;
		}
	}
}