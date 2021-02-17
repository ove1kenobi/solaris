#pragma once
#include "Planet.h"
#include "Player.h"
#include "Sun.h"
#include "GameObject.h"
#include "EventSystem/EventPublisher.h"
#include "Layer.h"
#include "EventSystem/RenderEvents.h"
#include <thread>
class Scene : public EventPublisher, public Layer
{
private:
	//GameObjects include planets, moons, asteroids, the Sun and the spaceship.
	int m_numPlanets;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Camera m_orthoCamera;
	PlayerCamera m_perspectiveCamera;
	Player m_player;
	std::vector<GameObject*> m_gameObjects;
public:
	Scene() noexcept;
	virtual ~Scene();
	bool init(unsigned int screenWidth, unsigned int screenHeight, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext);
	void Update() noexcept override;
	void OnEvent(IEvent& event) noexcept override;
	[[nodiscard]] const std::string GetDebugName() const noexcept override;
	void sendObjects();
};