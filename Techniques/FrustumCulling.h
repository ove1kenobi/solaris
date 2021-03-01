#pragma once
#include "../PlayerCamera.h"
#include "../GameObject.h"

struct RenderData
{
	std::vector<GameObject*> allObjects;
	std::vector<GameObject*> culledObjects;
	std::vector<GameObject*> culledPlanetsDepthSorted;
	size_t nrOfPlanetsInView;
	size_t totalNrOfPlanets;
	size_t totalNrOfObjects;
	size_t totalNrOfOrbits;
	bool sunCulled;
};

class FrustumCulling
{
private:
	DirectX::BoundingFrustum m_BoundingFrustum;
public:
	FrustumCulling() noexcept = default;
	virtual ~FrustumCulling() noexcept = default;
	[[nodiscard]] const bool Initialize(PlayerCamera& playerCamera) noexcept;
	void CullObjects(std::vector<GameObject*>& gameObjects, PlayerCamera& playerCamera, struct RenderData& renderData) noexcept;
};