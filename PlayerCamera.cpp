#include "PlayerCamera.h"

bool PlayerCamera::init(int screenWidth, int screenHeight) {
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_RELATIVE);
	EventBuss::Get().AddListener(this, EventType::MouseMoveEvent);
	this->m_screenFar = 10000;
	float FOV = 3.141592654f / this->m_FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	this->m_distanceFromShip = 1500.0f;
	
	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, this->m_screenNear, this->m_screenFar));
	return true;
}

void PlayerCamera::update(DirectX::XMVECTOR shipCoords) {
	this->m_forwardVector = shipCoords;
	
	this->m_posVector = DirectX::XMVectorSetX(this->m_posVector, sinf(DirectX::XMConvertToRadians(this->m_pitch)) * sinf(DirectX::XMConvertToRadians(this->m_yaw)) * this->m_distanceFromShip);
	this->m_posVector = DirectX::XMVectorSetY(this->m_posVector, cosf(DirectX::XMConvertToRadians(this->m_pitch)) * this->m_distanceFromShip);
	this->m_posVector = DirectX::XMVectorSetZ(this->m_posVector, -sinf(DirectX::XMConvertToRadians(this->m_pitch)) * cosf(DirectX::XMConvertToRadians(this->m_yaw)) * this->m_distanceFromShip);
	
	DirectX::XMStoreFloat4x4(&this->m_vMatrix, DirectX::XMMatrixLookAtLH(this->m_posVector, this->m_forwardVector, this->m_upVector));
}

void PlayerCamera::move(DirectX::XMVECTOR shipCoordsDiff) {
	this->m_posVector = DirectX::XMVectorAdd(shipCoordsDiff, this->m_posVector);
}

void PlayerCamera::mouseRot() {
	auto r = DirectX::Mouse::Get().GetState();
	float yValue = static_cast<float>(r.y);
	float xValue = static_cast<float>(r.x);
	
	this->m_yaw += xValue / 2;

	if (this->m_pitch < 180.0f && this->m_pitch > 0.0f)
		this->m_pitch += yValue / 2;
	if (this->m_pitch >= 180.0f)
		this->m_pitch = 179.9f;
	if (this->m_pitch <= 0.0f)
		this->m_pitch = 0.1f;
	
	
	/*
	DirectX::XMFLOAT4 result;
	DirectX::XMStoreFloat4(&result, DirectX::XMVectorSubtract(this->m_forwardVector, this->m_posVector));
	DirectX::XMFLOAT4 lookAt;
	DirectX::XMStoreFloat4(&lookAt, this->m_forwardVector);

	float yawRotSpeed = mouseX;
	float pitchRotSpeed = mouseY;

	float r = sqrt(result.x * result.x + result.y * result.y + result.z * result.z);
	float phi = acos(result.y / r);
	float theta = atan(result.z / result.x);
	
	phi += pitchRotSpeed;
	theta += yawRotSpeed;

	result.x = r * sin(phi) * sin(theta);
	result.y = r * cos(phi);
	result.z = r * -sin(phi) * cos(theta);

	result.x += lookAt.x;
	result.y += lookAt.y;
	result.z += lookAt.z;
	
	this->m_posVector = DirectX::XMLoadFloat4(&result);
	//DirectX::XMStoreFloat4x4(&this->m_vMatrix, DirectX::XMMatrixLookAtLH(this->m_posVector, this->m_forwardVector, this->m_upVector));
	*/
}

void PlayerCamera::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::MouseMoveEvent:
	{
		mouseRot();
		break;
	}
	}
}