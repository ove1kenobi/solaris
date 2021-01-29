#pragma once

#include <directxmath.h>

//Orthographic camera.
//sed as Base class for the PlayerCamera.
class Camera {
protected:
	float m_pitch, m_roll, m_yaw;

	//View Matrix
	DirectX::XMFLOAT4X4 m_vMatrix;
	//Projection Matrix
	DirectX::XMFLOAT4X4 m_pMatrix;

	//Position
	DirectX::XMVECTOR m_posVector;
	//Orientational vectors
	DirectX::XMVECTOR m_forwardVector, m_upVector, m_rightVector;

public:
	Camera() noexcept;
	~Camera();

	//Initializes the pMatrix & vMatrix of the orthographic camera.
	virtual bool init(int screenWidth, int screenHeight, float screenNear, float screenFar, float cameraHeight);

	//Changes the position of the camera.
	void setPos(float x, float y, float z);
	//Changes the rotation of the camera.
	void setRot(float p, float r, float y);

	//Return position.
	DirectX::XMFLOAT3 getPos();
	//Returns rotation.
	DirectX::XMFLOAT3 getRot();

	//Send in the view & projection matrices that you want to put the cameras matrices in.
	void getVMatrix(DirectX::XMMATRIX& vMatrix);
	void getPMatrix(DirectX::XMMATRIX& pMatrix);
};