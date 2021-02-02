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

class Player : public GameObject//, public IEventListener
{
private:
	// Model
	PlayerCamera* m_camera;
	Time m_time;

	DirectX::XMFLOAT3 m_forwardVector;
	DirectX::XMFLOAT3 m_rightVector;
	DirectX::XMFLOAT3 m_upVector;

	float m_speed, m_roation;

	void Move(DirectX::XMFLOAT3 direction);
	void YawRotation();

public:
	Player();
	~Player();

	void Initialize(DirectX::XMFLOAT3 position, PlayerCamera* camera);
	bool Update();
	//void OnEvent(const IEvent& event);
};

