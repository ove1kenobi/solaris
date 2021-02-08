#pragma once
#include "CosmicBody.h"
#include "Player.h"
#include "Sun.h"
#include "GameObject.h"
#include "EventSystem/EventPublisher.h"
#include "Layer.h"
#include "EventSystem/RenderEvents.h"
#include <imgui.h>
class Scene : public EventPublisher, public Layer
{
private:
	//GameObjects include planets, moons, asteroids and the sun.
	int m_numPlanets;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Camera m_orthoCamera;
	PlayerCamera m_perspectiveCamera;
	Player m_player;
	std::vector<GameObject*> m_gameObjects;

public:
	Scene() noexcept;
	~Scene();



	bool init(unsigned int screenWidth, unsigned int screenHeight, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext);
	void Update() noexcept override;

	void OnEvent(IEvent& event) noexcept override;
	[[nodiscard]] const std::string GetDebugName() const noexcept override;
	void sendObjects();
};
