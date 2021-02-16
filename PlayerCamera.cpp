#include "pch.h"
#include "PlayerCamera.h"

bool PlayerCamera::init(int screenWidth, int screenHeight) {
	//DirectXTK mouse
	EventBuss::Get().AddListener(this, EventType::MouseScrollEvent, EventType::ToggleImGuiEvent, EventType::RequestCameraEvent);
	this->m_screenFar = 100000.0f;
	float FOV = 3.141592654f / this->m_FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	m_orbitModeActive = true;

	//Initial camera distance from the ship.
	this->m_distanceFromShip = 60.0f;

	m_mousePosX = 0.0f;
	m_mousePosY = 0.0f;
	m_maxScroll = 100.0f;
	m_minScroll = 50.0f;
	m_pitch = M_PI_2;
	this->m_sensitivity = 0.2f;

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

	/*
	this->m_yaw -= xValue * (float)m_time.DeltaTime() / this->m_sensitivity;

	//Limits camera rotation up and down.
	if (this->m_pitch < M_PI && this->m_pitch > 0.0f)
		this->m_pitch -= yValue * (float)m_time.DeltaTime() / this->m_sensitivity;
	if (this->m_pitch >= M_PI)
		this->m_pitch = M_PI - 0.0001f;
	if (this->m_pitch <= 0.0f)
		this->m_pitch = 0.0001f;*/
	float alpha = 0.1f;

	m_yaw -= yaw;
	if (m_yaw >= 2 * M_PI) m_yaw -= 2 * M_PI;
	else if (m_yaw <= -2 * M_PI) m_yaw += 2 * M_PI;

	m_pitch += pitch;
	if (m_pitch > M_PI - alpha) m_pitch = M_PI - alpha;
	else if (m_pitch < alpha) m_pitch = alpha;
}

void PlayerCamera::mouseScroll(int scroll) {
	//Limits scroll between 100 to 50 atm..
	if(this->m_distanceFromShip < m_maxScroll && this->m_distanceFromShip > m_minScroll)
		this->m_distanceFromShip -= scroll / 10;
	if (this->m_distanceFromShip >= m_maxScroll)
		this->m_distanceFromShip = m_maxScroll - 0.1f;
	if (this->m_distanceFromShip <= m_minScroll)
		this->m_distanceFromShip = m_minScroll + 0.1f;
}

void PlayerCamera::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {

		/*
		case EventType::MouseMoveRelativeEvent:
		{
			int xDiff = static_cast<MouseMoveRelativeEvent*>(&event)->GetXDiff();
			int yDiff = static_cast<MouseMoveRelativeEvent*>(&event)->GetYDiff();
			if (m_orbitModeActive)
			{
				//mouseRot(xDiff, yDiff);
			}
			break;
		}
		case EventType::MouseMoveAbsoluteEvent:
		{
			float xCoord = static_cast<MouseMoveAbsoluteEvent*>(&event)->GetXCoord();
			float yCoord = static_cast<MouseMoveAbsoluteEvent*>(&event)->GetYCoord();
			if (m_orbitModeActive)
			{
				m_mousePosX = xCoord;
				m_mousePosY = yCoord;
				//mouseRot(xCoord, yCoord);
			}
			break;
		}*/
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