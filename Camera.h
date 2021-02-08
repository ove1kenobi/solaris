#pragma once
#include "time.h"
//Orthographic camera.
//Used as Base class for the PlayerCamera.
class Camera {
protected:
	float m_pitch, m_roll, m_yaw;

	DirectX::XMFLOAT4X4 m_vMatrix;
	DirectX::XMFLOAT4X4 m_pMatrix;

	DirectX::XMVECTOR m_posVector;
	//Orientational vectors
	DirectX::XMVECTOR m_forwardVector, m_upVector, m_rightVector;

	float m_screenNear = 0.1f;
	float m_screenFar;
	
	Time m_time;
public:
	Camera() noexcept;
	~Camera();

	//Initializes the pMatrix & vMatrix of the orthographic camera.
	virtual bool init(int screenWidth, int screenHeight, float cameraHeight);

	void setPos(float x, float y, float z);
	void setRot(float p, float r, float y);

	DirectX::XMFLOAT3 getPos();
	DirectX::XMFLOAT3 getRot();

	DirectX::XMMATRIX getVMatrix();
	DirectX::XMMATRIX getPMatrix();
};