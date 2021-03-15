#include "pch.h"
#include "PlayerCamera.h"

PlayerCamera::PlayerCamera() 
{
	m_distanceFromShip = 60.0f;
	m_maxScroll = 100.0f;
	m_minScroll = 50.0f;
	m_pitch = (float)M_PI_2;

	float alpha = 0.1f;
	m_maxPitch = (float)M_PI - alpha;
	m_minPitch = alpha;
	m_HitByAsteroid = false;
	m_ShakeDuration = -1.0;
	m_ShakeMagnitude = -1.0;
	m_ElapsedShakeTime = 0.0f;
	m_YawPriorToHit = 0.0f;
	m_PitchPriorToHit = 0.0f;
}

PlayerCamera::~PlayerCamera() {
	EventBuss::Get().RemoveListener(this, EventType::MouseScrollEvent);
	EventBuss::Get().RemoveListener(this, EventType::RequestCameraEvent);
	EventBuss::Get().RemoveListener(this, EventType::CameraShakeEvent);
}

bool PlayerCamera::init(int screenWidth, int screenHeight) {
	EventBuss::Get().AddListener(this, EventType::MouseScrollEvent, EventType::RequestCameraEvent, EventType::CameraShakeEvent);
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
	focusPos = DirectX::XMVectorAdd(DirectX::operator*(m_upVector, 15.0f), focusPos);

	// Create forward vector 
	m_forwardVector = DirectX::XMVectorSubtract(focusPos, m_posVector);
	m_forwardVector = DirectX::XMVector3Normalize(m_forwardVector);

	// Create the view matrix
	DirectX::XMStoreFloat4x4(&m_vMatrix, DirectX::XMMatrixLookAtLH(m_posVector, focusPos, m_upVector));

	if (m_HitByAsteroid == true)
	{	//Do the shake on the orbit, follow up with a focus shake:
		DirectX::XMFLOAT3 newFocus;
		DirectX::XMStoreFloat3(&newFocus, focusPos);
		newFocus = Shake(newFocus);
		DirectX::XMStoreFloat4x4(&m_vMatrix, DirectX::XMMatrixLookAtLH(m_posVector, DirectX::XMLoadFloat3(&newFocus), m_upVector));
	}
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
	//Limits scroll between 100 to 50
	if(this->m_distanceFromShip < m_maxScroll && this->m_distanceFromShip > m_minScroll)
		this->m_distanceFromShip -= scroll / 10;
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

const DirectX::XMFLOAT3& PlayerCamera::Shake(DirectX::XMFLOAT3& focus)
{
	//Randomize shake for both orbit rotation and focus position:
	using t_clock = std::chrono::high_resolution_clock;
	std::default_random_engine gen(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	OrbitRotation(static_cast<float>(dist(gen)) * 0.02f , static_cast<float>(dist(gen)) * 0.02f);
	
	m_ElapsedShakeTime += static_cast<float>(m_time.DeltaTime());
	if (m_ElapsedShakeTime > m_ShakeDuration)
	{
		m_HitByAsteroid = false;
		m_ElapsedShakeTime = 0.0f;
		m_yaw = m_YawPriorToHit;
		m_pitch = m_PitchPriorToHit;
	}
	focus.x = focus.x + dist(gen) * m_ShakeMagnitude;
	focus.y = focus.y + dist(gen) * m_ShakeMagnitude;
	focus.z = focus.z + dist(gen) * m_ShakeMagnitude;
	return focus;
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
		case EventType::CameraShakeEvent:
		{
			CameraShakeEvent& derivedEvent = static_cast<CameraShakeEvent&>(event);
			m_HitByAsteroid = true;
			m_ShakeDuration = derivedEvent.GetDuration();
			m_ShakeMagnitude = derivedEvent.GetMagnitude();
			m_YawPriorToHit = m_yaw;
			m_PitchPriorToHit = m_pitch;
			break;
		}
	}
}