#include "PlayerCamera.h"

bool PlayerCamera::init(int screenWidth, int screenHeight) {
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);
	EventBuss::Get().AddListener(this, EventType::MouseMoveEvent, EventType::MouseScrollEvent);
	this->m_screenFar = 100000.0f;
	float FOV = 3.141592654f / this->m_FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	this->m_distanceFromShip = 300.0f;
	this->m_sensitivity = 2.0f;
	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, this->m_screenNear, this->m_screenFar));
	return true;
}

void PlayerCamera::update(DirectX::XMVECTOR shipCoords) {
	this->m_forwardVector = shipCoords;
	
	this->m_posVector = DirectX::XMVectorSetX(this->m_posVector, sinf(this->m_pitch) * sinf(this->m_yaw) * this->m_distanceFromShip);
	this->m_posVector = DirectX::XMVectorSetY(this->m_posVector, cosf(this->m_pitch) * this->m_distanceFromShip);
	this->m_posVector = DirectX::XMVectorSetZ(this->m_posVector, -sinf(this->m_pitch) * cosf(this->m_yaw) * this->m_distanceFromShip);

	this->m_posVector = DirectX::XMVectorAdd(this->m_posVector, this->m_forwardVector);
	DirectX::XMStoreFloat4x4(&this->m_vMatrix, DirectX::XMMatrixLookAtLH(this->m_posVector, this->m_forwardVector, this->m_upVector));
}

void PlayerCamera::mouseRot() {
	auto r = DirectX::Mouse::Get().GetState();
	float yValue = static_cast<float>(r.y);
	float xValue = static_cast<float>(r.x);
	
	float pi = static_cast<float>((atan(1) * 4));

	this->m_yaw -= xValue * (float)m_time.DeltaTime() / this->m_sensitivity;

	if (this->m_pitch < pi && this->m_pitch > 0.0f)
		this->m_pitch -= yValue * (float)m_time.DeltaTime() / this->m_sensitivity;
	if (this->m_pitch >= pi)
		this->m_pitch = pi - 0.0001f;
	if (this->m_pitch <= 0.0f)
		this->m_pitch = 0.0001f;
}

void PlayerCamera::mouseScroll() {
	int scroll = DirectX::Mouse::Get().GetState().scrollWheelValue;
	DirectX::Mouse::Get().ResetScrollWheelValue();
	if(this->m_distanceFromShip < 600.0f && this->m_distanceFromShip > 50.0f)
		this->m_distanceFromShip -= scroll  / 3;
	if (this->m_distanceFromShip >= 600.0f)
		this->m_distanceFromShip = 600.0f - 0.1f;
	if (this->m_distanceFromShip <= 50.0f)
		this->m_distanceFromShip = 50.0f + 0.1f;
}

void PlayerCamera::shipRot(float step) {
	this->m_yaw +=  step/* * 3 / 4*/;
}

void PlayerCamera::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::MouseMoveEvent:
	{
		mouseRot();
		break;
	}
	case EventType::MouseScrollEvent:
	{
		mouseScroll();
		break;
	}
	}
}