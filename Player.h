#pragma once
#include "time.h"
#include "PlayerCamera.h"
#include "GameObject.h"
#include "SpaceShip.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventBuss.h"
#include "EventSystem/InputEvents.h"
class Player : public IEventListener
{
private:
	// Reference to the game object that is the ship and that has the model.
	SpaceShip* m_ship;
	// Reference to the camera that is connected to the player/ship.
	PlayerCamera* m_camera;

	bool m_moveForwards, m_moveBackwards;
	bool m_moveUp, m_moveDown;
	bool m_rotateRight, m_rotateLeft;
	
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
	SpaceShip* getShip();

	void OnEvent(IEvent& event) noexcept;
};

