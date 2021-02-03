#pragma once
#define _USE_MATH_DEFINES

#include <d3d11.h>
#include <DirectXMath.h>
#include <cmath>
#include "time.h"
#include "PlayerCamera.h"
#include "GameObject.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventBuss.h"
#include "EventSystem/MouseMoveEvent.h"
#include "EventSystem/MouseButtenEvent.h"
#include "EventSystem/KeyboardEvent.h"

#include <iostream>					// Remove

class Player : public GameObject, public IEventListener
{
private:
	// Model

	bool m_moveForwards, m_moveBackwards;
	bool m_moveUp, m_moveDown;
	bool m_rotateRight, m_rotateLeft;

	PlayerCamera* m_camera;
	Time m_time;

	DirectX::XMFLOAT3 m_forwardVector;
	DirectX::XMFLOAT3 m_rightVector;
	DirectX::XMFLOAT3 m_upVector;

	float m_speed, m_rotation;

	void Move(DirectX::XMFLOAT3 direction);
	void YawRotation(float rotation);

public:
	Player();
	~Player();

	bool Initialize(/*DirectX::XMFLOAT3 position,*/ PlayerCamera* camera);
	bool update();
	void OnEvent(IEvent& event) noexcept;
};

