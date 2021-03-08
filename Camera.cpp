#include "pch.h"
#include "Camera.h"

Camera::Camera() noexcept
	: m_pitch{ 0 },
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
	},
	m_screenFar{ 0 }
{
}

Camera::~Camera() {

}

//Init for an orthographic camera.
bool Camera::init(int screenWidth, int screenHeight, float cameraHeight) {
	this->m_screenFar = 10000.0f;
	DirectX::XMStoreFloat4x4(&this->m_pMatrix, DirectX::XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), this->m_screenNear, this->m_screenFar));

	//Above the sun.
	this->setPos(0.0f, cameraHeight, 0.0f);
	//Look down
	this->setRot(static_cast<float>(M_PI), 0.0f, 0.0f); //Might be wrong. Will have to test when implementing minimap.

	//Setting the view matrix. Doing it in init since it will not have to be updated for the static ortho camera.
	//Create the rotation matrix.
	DirectX::XMMATRIX rotMatrixX;
	rotMatrixX = DirectX::XMMatrixRotationX(this->m_pitch);

	//Rotate the axis that get affected by the pitch change.
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

DirectX::XMFLOAT4X4 Camera::getVMatrix() {
	return this->m_vMatrix;
}

DirectX::XMFLOAT4X4 Camera::getPMatrix() {
	return m_pMatrix;
}
