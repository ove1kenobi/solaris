#pragma once
#include "Planet.h"
#include "Player.h"
#include "Sun.h"
#include "GameObject.h"
#include "Asteroid.h"
#include "EventSystem/EventPublisher.h"
#include "Layer.h"
#include "EventSystem/RenderEvents.h"
#include "EventSystem/UIEvents.h"
#include "Techniques/MousePicking.h"
#include "Techniques/FrustumCulling.h"
#include "ourMath.h"
class fstream;
class Scene : public EventPublisher, public Layer
{
private:
	//GameObjects include planets, moons, asteroids, the Sun and the spaceship.
	size_t m_numPlanets;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Camera m_orthoCamera;
	PlayerCamera m_perspectiveCamera;
	Player m_player;
	std::vector<GameObject*> m_gameObjects;
	std::vector<Planet*> m_planets; //USED FOR SENDING DATA TO THE SHADER
	std::vector<WaterSphere*> m_waterSpheres;
	std::vector<Planet*> m_radioactivePlanets;
	Sun* m_sun;
	MousePicking m_Picking;
	FrustumCulling m_FrustumCulling;
	RenderData m_RenderData;

	Time m_time;
	long double m_nextAstroSpawnTime;
	long double m_damageTimer;

	size_t m_persistentObjEnd;

	//Invincibility data:
	float m_ElapsedTime;
	static constexpr float m_InvincibilityDuration = 0.5f;
	bool m_IsInvincible;

private:
	[[nodiscard]] const std::vector<std::wstring> RandomizePlanetNames(std::default_random_engine generator) noexcept;
public:
	Scene() noexcept;
	virtual ~Scene();
	bool init(unsigned int screenWidth, unsigned int screenHeight, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext);
	void Update() noexcept override;
	void OnEvent(IEvent& event) noexcept override;
	[[nodiscard]] const std::string GetDebugName() const noexcept override;
	void sendObjects();
	void AddGameObject(GameObject* obj);
	void RemoveGameObject(GameObject* obj);
	void CheckForCollisions() noexcept;
	int GetPlayerHealth();
};