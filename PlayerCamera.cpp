#include "pch.h"
#include "PlayerCamera.h"

PlayerCamera::PlayerCamera() {
	m_distanceFromShip = 60.0f;
	m_mousePosX = 0.0f;
	m_mousePosY = 0.0f;
	m_maxScroll = 100.0f;
	m_minScroll = 50.0f;
	m_pitch = (float)M_PI_2;
}

bool PlayerCamera::init(int screenWidth, int screenHeight) {
	//DirectXTK mouse
	EventBuss::Get().AddListener(this, EventType::MouseScrollEvent, EventType::RequestCameraEvent);
	this->m_screenFar = 100000.0f;
	float FOV = 3.141592654f / this->m_FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, this->m_screenNear, this->m_screenFar));
	return true;
}

void PlayerCamera::update(DirectX::XMVECTOR shipCoords) {
	//Orbit around the ship.
	DirectX::XMVECTOR focusPos = shipCoords;

	m_posVector = DirectX::XMVectorSetX(m_posVector, sinf(m_pitch) * sinf(m_yaw) * m_distanceFromShip);
	m_posVector = DirectX::XMVectorSetY(m_posVector, cosf(m_pitch) * m_distanceFromShip);
	m_posVector = DirectX::XMVectorSetZ(m_posVector, -sinf(m_pitch) * cosf(m_yaw) * m_distanceFromShip);
	m_posVector = DirectX::XMVectorAdd(m_posVector, focusPos);

	m_forwardVector = DirectX::XMVectorSubtract(focusPos, m_posVector);
	m_forwardVector = DirectX::XMVector3Normalize(m_forwardVector);

	//focusPos = DirectX::XMVectorAdd(DirectX::operator*(m_upVector, 10.0f), focusPos);

	DirectX::XMStoreFloat4x4(&m_vMatrix, DirectX::XMMatrixLookAtLH(m_posVector, focusPos, m_upVector));
}

void PlayerCamera::OrbitRotation(float yaw, float pitch) {

	float alpha = 0.1f;

	m_yaw -= yaw;
	if (m_yaw >= 2.0f * (float)M_PI) m_yaw -= 2.0f * (float)M_PI;
	else if (m_yaw <= -2.0f * (float)M_PI) m_yaw += 2.0f * (float)M_PI;

	m_pitch += pitch;
	if (m_pitch > (float)M_PI - alpha) m_pitch = (float)M_PI - alpha;
	else if (m_pitch < alpha) m_pitch = alpha;
}

void PlayerCamera::mouseScroll(int scroll) {
	//Limits scroll between 100 to 50
	if(this->m_distanceFromShip < m_maxScroll && this->m_distanceFromShip > m_minScroll)
		this->m_distanceFromShip -= scroll / 10;
	if (this->m_distanceFromShip >= m_maxScroll)
		this->m_distanceFromShip = m_maxScroll - 0.1f;
	if (this->m_distanceFromShip <= m_minScroll)
		this->m_distanceFromShip = m_minScroll + 0.1f;
}

void PlayerCamera::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		case EventType::MouseScrollEvent:
		{
			int wheelScroll = static_cast<MouseScrollEvent*>(&event)->GetWheelScroll();
			mouseScroll(wheelScroll);
			break;
		}
		case EventType::RequestCameraEvent:
		{
			DelegateCameraEvent event(this);
			EventBuss::Get().Delegate(event);
			break;
		}
	}
}