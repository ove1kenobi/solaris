#pragma once

#include <directxmath.h>

//Orthographic camera.
//sed as Base class for the PlayerCamera.
class Camera {
protected:
	float my_pitch, my_roll, my_yaw;

	//View Matrix
	DirectX::XMFLOAT4X4 my_vMatrix;
	//Projection Matrix
	DirectX::XMFLOAT4X4 my_pMatrix;

	//Position
	DirectX::XMVECTOR my_posVector;
	//Orientational vectors
	DirectX::XMVECTOR my_forwardVector, my_upVector, my_rightVector;

public:
	Camera();
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