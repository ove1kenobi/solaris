#include "PlayerCamera.h"

PlayerCamera::PlayerCamera() {

}

PlayerCamera::~PlayerCamera() {

}

bool PlayerCamera::init(int screenWidth, int screenHeight, float screenNear, float screenFar, float FOVvalue) {
	float FOV = 3.141592654f / FOVvalue;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	DirectX::XMStoreFloat4x4(&this->my_pMatrix, DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, screenNear, screenFar));
}

void PlayerCamera::update(DirectX::XMVECTOR shipCoords) {
	//Lookat is same as forward atm.
	DirectX::XMVECTOR lookAt = shipCoords;
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	float rad = 0.0174532925;
	float pitch = this->my_pitch * rad;
	float roll = this->my_roll * rad;
	float yaw = this->my_yaw * rad;

	//Create the rotation matrices.
	DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	DirectX::XMMATRIX rotMatrixX = DirectX::XMMatrixRotationX(pitch);
	DirectX::XMMATRIX rotMatrixY = DirectX::XMMatrixRotationY(yaw);

	//Update the cameras vectors.
	this->my_forwardVector = DirectX::XMVector3TransformCoord(forward, rotMatrixY);
	this->my_rightVector = DirectX::XMVector3TransformCoord(right, rotMatrixY);

	//Update lookAt
	lookAt = DirectX::XMVector3TransformCoord(lookAt, rotMatrix);
	up = DirectX::XMVector3TransformCoord(up, rotMatrix);

	//Oklart om denna behövs
	lookAt = DirectX::XMVectorAdd(this->my_posVector, lookAt);

	//Update the vMatrix
	DirectX::XMStoreFloat4x4(&this->my_vMatrix, DirectX::XMMatrixLookAtLH(this->my_posVector, lookAt, up));
}

void PlayerCamera::move(DirectX::XMVECTOR shipCoordsDiff) {
	this->my_posVector = DirectX::XMVectorAdd(shipCoordsDiff, this->my_posVector);
}

void PlayerCamera::mouseRot(int mouseX, int mouseY) {
	if (mouseY < 1000 && mouseY > -1000) {
		this->my_pitch = mouseY * 0.1f;
	}
	this->my_roll = mouseX * 0.1f;
}