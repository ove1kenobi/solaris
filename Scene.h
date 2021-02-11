#pragma once
#include "Planet.h"
#include "Player.h"
#include "Sun.h"
#include "GameObject.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include <imgui.h>
#include <thread>

class Scene : public EventPublisher, IEventListener
{
private:
	//GameObjects include planets, moons, asteroids and the sun.
	int m_numPlanets;
	Camera m_orthoCamera;
	PlayerCamera m_perspectiveCamera;
	Player m_player;
	std::vector<GameObject*> m_gameObjects;
public:
	Scene() noexcept;
	~Scene();

	bool init(unsigned int screenWidth, unsigned int screenHeight);
	bool update(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);

	void OnEvent(IEvent& event) noexcept;
	void sendObjects();
};