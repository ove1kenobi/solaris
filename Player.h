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
	Time m_time;

	float m_mousePosX, m_mousePosY;
	bool m_moveForwards, m_moveBackwards;
	bool m_playerControlsActive;
	float m_speed, m_rotationSpeed;
	// Updates the rotation for the camera and the ship
	void UpdateRotation();

public:
	Player();
	~Player();

	bool Initialize(PlayerCamera* camera);
	bool update();
	SpaceShip* getShip();

	void OnEvent(IEvent& event) noexcept;
};

