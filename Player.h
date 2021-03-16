#pragma once
#include "time.h"
#include "PlayerCamera.h"
#include "GameObject.h"
#include "SpaceShip.h"
#include "Planet.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem\EventPublisher.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem/SoundEvents.h"
#include "EventSystem/UIEvents.h"
#include "Resources.h"
#include "EventSystem\UtilityEvents.h"

struct PlayerInfo
{
	Planet* closestPlanet;
	Planet* planetInteractedWith;
	float distanceToClosestPlanet;
	float fuelPercentage;
	float oxygenPercentage;
	float HealthPercentage;
	int storageUsage;
	int storageCapacity;
};

class Player : public IEventListener, public EventPublisher
{
private:
	// Reference to the game object that is the ship and that has the model.
	SpaceShip* m_ship;
	// Reference to the camera that is connected to the player/ship.
	PlayerCamera* m_camera;
	Time m_time;

	// Resources
	int m_fuelCapacity;
	int m_oxygenCapacity;
	int m_healthCapacity;
	int m_storageCapacity;
	int m_storageUsage;
	int m_resources[numberOfResources];

	// Movement
	PlayerInfo m_PlayerInfo;
	float m_thrusterForce, m_desiredSpeed, m_topSpeed;
	DirectX::XMFLOAT3 m_TetheredDistanceToUphold;
	DirectX::XMFLOAT3 m_PreviousCenterPosition;

	// Input 
	float m_mousePosX, m_mousePosY;
	bool m_moveForwards, m_moveBackwards;
	bool m_stopMovement;
	bool m_playerControlsActive, m_stabilizerActive;
	float m_rotationSpeed;
	bool m_TetheredToClosestPlanet;
private:
	void DetermineClosestPlanet(const std::vector<Planet*>& planets) noexcept;
	// Updates the rotation for the camera and the ship
	void UpdateRotation();
	DirectX::XMFLOAT3 Stabilize();
public:
	Player();
	virtual ~Player();

	bool Initialize(PlayerCamera* camera);
	bool update(const std::vector<Planet*>& planets);
	SpaceShip* getShip();
	void AddResource(int amount, Resource resource);

	void OnEvent(IEvent& event) noexcept;
	void DelegatePlayerInfo() noexcept;
	int GetHealth() noexcept;
	void UpdateHealth(int value);
	int GetMaxHealth() noexcept;
	void UpdateMaxHealth(int value);
	void Kill() noexcept;
};

