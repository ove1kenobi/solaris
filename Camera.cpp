#include "Camera.h"

Camera::Camera() noexcept
	:	m_pitch{ 0 },
		m_roll{ 0 },
		m_yaw{ 0 },
		m_posVector{ DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },
		m_rightVector{ DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },
		m_upVector{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },
		m_forwardVector{ DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) },
		m_vMatrix{
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
		},
		m_pMatrix{
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
		}
{
	
}

Camera::~Camera() {

}

//Init for an orthographic camera.
bool Camera::init(int screenWidth, int screenHeight, float screenNear, float screenFar, float cameraHeight) {
	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), screenNear, screenFar));

	//Above the sun.
	this->setPos(0.0f, cameraHeight, 0.0f);
	//Look down
	this->setRot(-90.0f, 0.0f, 0.0f);

	//Setting the view matrix. Doing it in init since it will not have to be updated for the static ortho camera.
	DirectX::XMMATRIX rotMatrixX;
	float pRad = this->m_pitch * 0.0174532925f; //Pitch in radians.

	//Create the rotation matrix.
	rotMatrixX = DirectX::XMMatrixRotationX(pRad);

	//Rotate the two axis that get affected by the pitch change.
	this->m_forwardVector = DirectX::XMVector3TransformCoord(this->m_forwardVector, rotMatrixX);

	DirectX::XMStoreFloat4x4(&this->m_vMatrix, DirectX::XMMatrixLookAtLH(this->m_posVector, this->m_forwardVector, this->m_upVector));

	return true;
}

void Camera::setPos(float x, float y, float z) {
	this->m_posVector = DirectX::XMVectorSet(x, y, z, 0.0f);
}

void Camera::setRot(float p, float r, float y) {
	this->m_pitch = p;
	this->m_roll = r;
	this->m_yaw = y;
}

DirectX::XMFLOAT3 Camera::getPos() {
	return DirectX::XMFLOAT3(
		DirectX::XMVectorGetX(this->m_posVector),
		DirectX::XMVectorGetY(this->m_posVector),
		DirectX::XMVectorGetZ(this->m_posVector)
	);
}

DirectX::XMFLOAT3 Camera::getRot() {
	return DirectX::XMFLOAT3(this->m_pitch, this->m_roll, this->m_yaw);
}

void Camera::getVMatrix(DirectX::XMMATRIX& vMatrix) {
	vMatrix = DirectX::XMLoadFloat4x4(&this->m_vMatrix);
}

void Camera::getPMatrix(DirectX::XMMATRIX& pMatrix) {
	pMatrix = DirectX::XMLoadFloat4x4(&this->m_pMatrix);
}