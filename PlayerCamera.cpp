#include "PlayerCamera.h"

bool PlayerCamera::init(int screenWidth, int screenHeight, float screenNear, float screenFar, float FOVvalue) {
	float FOV = 3.141592654f / FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, screenNear, screenFar));
	return true;			// TODO: added by Ove due to "missing return value" error
}

void PlayerCamera::update(DirectX::XMVECTOR shipCoords) {
	//Lookat is same as forward atm.
	DirectX::XMVECTOR lookAt = shipCoords;
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

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
	lookAt = DirectX::XMVector3TransformCoord(lookAt, rotMatrix);
	up = DirectX::XMVector3TransformCoord(up, rotMatrix);

	//Oklart om denna behövs
	lookAt = DirectX::XMVectorAdd(this->m_posVector, lookAt);

	//Update the vMatrix
	DirectX::XMStoreFloat4x4(&this->m_vMatrix, DirectX::XMMatrixLookAtLH(this->m_posVector, lookAt, up));
}

void PlayerCamera::move(DirectX::XMVECTOR shipCoordsDiff) {
	this->m_posVector = DirectX::XMVectorAdd(shipCoordsDiff, this->m_posVector);
}

void PlayerCamera::mouseRot(int mouseX, int mouseY) {
	if (mouseY < 1000 && mouseY > -1000) {
		this->m_pitch = mouseY * 0.1f;
	}
	this->m_roll = mouseX * 0.1f;
}