#include "PlayerCamera.h"

bool PlayerCamera::init(int screenWidth, int screenHeight) {
	EventBuss::Get().AddListener(this, EventType::MouseMoveEvent);
	this->m_screenFar = 10000;
	float FOV = 3.141592654f / this->m_FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, this->m_screenNear, this->m_screenFar));
	DirectX::XMVECTOR param = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	this->update(param);
	return true;			// TODO: added by Ove due to "missing return value" error
}

void PlayerCamera::update(DirectX::XMVECTOR shipCoords) {
	//Lookat is same as forward atm.
	DirectX::XMVECTOR lookAt = shipCoords;
	this->m_forwardVector = shipCoords;
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	/*
	float rad = 0.0174532925f;
	float pitch = this->m_pitch * rad;
	float roll = this->m_roll * rad;
	float yaw = this->m_yaw * rad;

	//Create the rotation matrices.
	DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	DirectX::XMMATRIX rotMatrixX = DirectX::XMMatrixRotationX(pitch);
	DirectX::XMMATRIX rotMatrixY = DirectX::XMMatrixRotationY(yaw);

	//Update the cameras vectors.
	this->m_forwardVector = DirectX::XMVector3TransformCoord(forward, rotMatrixY);
	this->m_rightVector = DirectX::XMVector3TransformCoord(right, rotMatrixY);
	
	//Update lookAt
	//lookAt = DirectX::XMVector3TransformCoord(lookAt, rotMatrix);
	up = DirectX::XMVector3TransformCoord(up, rotMatrix);
	*/
	//Update the vMatrix
	DirectX::XMStoreFloat4x4(&this->m_vMatrix, DirectX::XMMatrixLookAtLH(this->m_posVector, lookAt, up));
}

void PlayerCamera::move(DirectX::XMVECTOR shipCoordsDiff) {
	this->m_posVector = DirectX::XMVectorAdd(shipCoordsDiff, this->m_posVector);
}

void PlayerCamera::mouseRot(int mouseX, int mouseY) {
	DirectX::XMFLOAT4 result;
	DirectX::XMStoreFloat4(&result, DirectX::XMVectorSubtract(this->m_posVector, this->m_forwardVector));

	float yawRotSpeed = mouseX * 0.1f;
	float pitchRotSpeed = mouseY * 0.1f;
	float stepX = yawRotSpeed * (float)m_time.DeltaTime();
	float stepY = pitchRotSpeed * (float)m_time.DeltaTime();
	// Rotating right and forward vector in a 2D plane
	//m_rightVector.x = cos(step) * m_rightVector.x - sin(step) * m_rightVector.z;
	//m_rightVector.z = sin(step) * m_rightVector.x + cos(step) * m_rightVector.z;

	//X-rotate
	result.x = cos(stepX) * result.x - sin(stepX) * result.z;
	result.z = sin(stepX) * result.x + cos(stepX) * result.z;
	/*
	//Y-rotate
	result x =
	result y = 
	result z = 
	*/
	this->m_posVector = DirectX::XMLoadFloat4(&result);
}

void PlayerCamera::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
	case EventType::MouseMoveEvent:
	{
		int xCoord = static_cast<MouseMoveEvent*>(&event)->GetXCoord();
		int yCoord = static_cast<MouseMoveEvent*>(&event)->GetYCoord();

		mouseRot(xCoord, yCoord);
		break;
	}
	}
}