#pragma once
#include "../PlayerCamera.h"
#include "../GameObject.h"
#include "../SpaceShip.h"

class WaterSphere;

struct RenderData
{
	std::vector<GameObject*>* allObjects;
	std::vector<GameObject*> culledObjects;
	std::vector<GameObject*> culledPlanetsDepthSorted;
	std::vector<WaterSphere*>* waterSpheres;
	size_t nrOfPlanetsInView;
	size_t totalNrOfPlanets;
	size_t totalNrOfObjects;
	size_t totalNrOfOrbits;
	size_t nrOfAsteroidsInView;
	size_t totalNrOfAsteroids;
	bool sunCulled;
};

class FrustumCulling
{
private:
	DirectX::BoundingFrustum m_BoundingFrustum;
	DirectX::BoundingFrustum m_ShadowBoundingFrustum;
public:
	FrustumCulling() noexcept = default;
	virtual ~FrustumCulling() noexcept = default;
	[[nodiscard]] const bool Initialize(PlayerCamera& playerCamera) noexcept;
	void CreateShadowFrustum(DirectX::XMMATRIX shadowProjectionMatrix) noexcept;
	void CullObjects(std::vector<GameObject*>& gameObjects, PlayerCamera& playerCamera, struct RenderData& renderData) noexcept;
	[[nodiscard]] std::vector<GameObject*> CullShadowObjects(std::vector<GameObject*>* gameObjects, const DirectX::XMMATRIX& viewMatrix, const size_t& nrOfPlanets) noexcept;
};