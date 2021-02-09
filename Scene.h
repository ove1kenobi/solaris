#pragma once
#include "CosmicBody.h"
#include "Player.h"
#include "Sun.h"
#include "GameObject.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include <imgui.h>
class Scene : public EventPublisher, IEventListener
{
private:
	//GameObjects include planets, moons, asteroids, the Sun and the spaceship.
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
