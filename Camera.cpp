#include "Camera.h"

Camera::Camera() {
	this->my_pitch	= 0;
	this->my_roll	= 0;
	this->my_yaw	= 0;

	this->my_posVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	this->my_rightVector	= DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	this->my_upVector		= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	this->my_forwardVector	= DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {

}

//Init for an orthographic camera.
bool Camera::init(int screenWidth, int screenHeight, float screenNear, float screenFar, float cameraHeight) {
	DirectX::XMStoreFloat4x4(&this->my_pMatrix, DirectX::XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), screenNear, screenFar));

	//Above the sun.
	this->setPos(0.0f, cameraHeight, 0.0f);
	//Look down
	this->setRot(-90.0f, 0.0f, 0.0f);

	//Setting the view matrix. Doing it in init since it will not have to be updated for the static ortho camera.
	DirectX::XMMATRIX rotMatrix, rotMatrixX;
	float pRad = this->my_pitch * 0.0174532925f; //Pitch in radians.

	//Create the rotation matrix.
	rotMatrixX = DirectX::XMMatrixRotationX(pRad);

	//Rotate the two axis that get affected by the pitch change.
	this->my_forwardVector = DirectX::XMVector3TransformCoord(this->my_forwardVector, rotMatrixX);

	DirectX::XMStoreFloat4x4(&this->my_vMatrix, DirectX::XMMatrixLookAtLH(this->my_posVector, this->my_forwardVector, this->my_upVector));

	return true;
}

void Camera::setPos(float x, float y, float z) {
	this->my_posVector = DirectX::XMVectorSet(x, y, z, 0.0f);
}

void Camera::setRot(float p, float r, float y) {
	this->my_pitch = p;
	this->my_roll = r;
	this->my_yaw = y;
}

DirectX::XMFLOAT3 Camera::getPos() {
	return DirectX::XMFLOAT3(
		DirectX::XMVectorGetX(this->my_posVector),
		DirectX::XMVectorGetY(this->my_posVector),
		DirectX::XMVectorGetZ(this->my_posVector)
	);
}

DirectX::XMFLOAT3 Camera::getRot() {
	return DirectX::XMFLOAT3(this->my_pitch, this->my_roll, this->my_yaw);
}

void Camera::getVMatrix(DirectX::XMMATRIX& vMatrix) {
	vMatrix = DirectX::XMLoadFloat4x4(&this->my_vMatrix);
}

void Camera::getPMatrix(DirectX::XMMATRIX& pMatrix) {
	pMatrix = DirectX::XMLoadFloat4x4(&this->my_pMatrix);
}