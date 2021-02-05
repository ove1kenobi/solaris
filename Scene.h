#pragma once
#include "CosmicBody.h"
#include "Player.h"
#include "Sun.h"
#include "SpaceShip.h"
#include "GameObject.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include <random>
#include "Time.h"
#include <imgui.h>
class Scene : public EventPublisher, IEventListener
{
private:
	//GameObjects include planets, moons, asteroids and the sun.
	int m_numPlanets;
	Camera m_orthoCamera;
	Player m_player;

	//ModelFactory m_factory;
public:
	Scene() noexcept;
	~Scene();

	std::vector<GameObject*> m_gameObjects;

	bool init(unsigned int screenWidth, unsigned int screenHeight);
	bool update(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);

	void OnEvent(IEvent& event) noexcept;
	void sendObjects();

	PlayerCamera m_perspectiveCamera;
};
