#include "pch.h"
#include "PlayerCamera.h"

PlayerCamera::PlayerCamera() {
	m_distanceFromShip = 1.6f;
	m_maxScroll = 3.0f;
	m_minScroll = 1.3f;
	m_pitch = (float)M_PI_2;

	float alpha = 0.1f;
	m_maxPitch = (float)M_PI - alpha;
	m_minPitch = alpha;
}

PlayerCamera::~PlayerCamera() {
	EventBuss::Get().RemoveListener(this, EventType::MouseScrollEvent);
	EventBuss::Get().RemoveListener(this, EventType::RequestCameraEvent);
}

bool PlayerCamera::init(int screenWidth, int screenHeight) {
	EventBuss::Get().AddListener(this, EventType::MouseScrollEvent, EventType::RequestCameraEvent);
	this->m_screenFar = 100000.0f;
	float FOV = 3.141592654f / this->m_FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, this->m_screenNear, this->m_screenFar));
	return true;
}

void PlayerCamera::update(DirectX::XMVECTOR shipCoords) {
	//Orbit around the ship.

	// Set focus point to the ship position
	DirectX::XMVECTOR focusPos = shipCoords;

	// Calculate the camera position in relation to the ship and rotation with polar coordinates
	m_posVector = DirectX::XMVectorSetX(m_posVector, sinf(m_pitch) * sinf(m_yaw) * m_distanceFromShip);
	m_posVector = DirectX::XMVectorSetY(m_posVector, cosf(m_pitch) * m_distanceFromShip);
	m_posVector = DirectX::XMVectorSetZ(m_posVector, -sinf(m_pitch) * cosf(m_yaw) * m_distanceFromShip);
	m_posVector = DirectX::XMVectorAdd(m_posVector, focusPos);

	// Offsets the ship from the camera center
	focusPos = DirectX::XMVectorAdd(DirectX::operator*(m_upVector, m_distanceFromShip * 0.2f), focusPos);

	// Create forward vector 
	m_forwardVector = DirectX::XMVectorSubtract(focusPos, m_posVector);
	m_forwardVector = DirectX::XMVector3Normalize(m_forwardVector);

	// Create the view matrix
	DirectX::XMStoreFloat4x4(&m_vMatrix, DirectX::XMMatrixLookAtLH(m_posVector, focusPos, m_upVector));
}

void PlayerCamera::OrbitRotation(float yaw, float pitch) {
	m_yaw -= yaw;
	if (m_yaw >= 2.0f * (float)M_PI) m_yaw -= 2.0f * (float)M_PI;
	else if (m_yaw <= -2.0f * (float)M_PI) m_yaw += 2.0f * (float)M_PI;

	m_pitch += pitch;
	if (m_pitch > m_maxPitch) m_pitch = m_maxPitch;
	else if (m_pitch < m_minPitch) m_pitch = m_minPitch;
}

void PlayerCamera::mouseScroll(int scroll) {
	//Limits scroll between 5 to 1
	if(this->m_distanceFromShip < m_maxScroll && this->m_distanceFromShip > m_minScroll)
		this->m_distanceFromShip -= scroll / 1000.0f;
	if (this->m_distanceFromShip >= m_maxScroll)
		this->m_distanceFromShip = m_maxScroll - 0.1f;
	if (this->m_distanceFromShip <= m_minScroll)
		this->m_distanceFromShip = m_minScroll + 0.1f;
}

DirectX::XMFLOAT3 PlayerCamera::GetForwardVector()
{
	return DirectX::XMFLOAT3(
		DirectX::XMVectorGetX(m_forwardVector),
		DirectX::XMVectorGetY(m_forwardVector),
		DirectX::XMVectorGetZ(m_forwardVector)
	);
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