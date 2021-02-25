#pragma once
#include "../PlayerCamera.h"
#include "../GameObject.h"
class FrustumCulling
{
private:
	DirectX::BoundingFrustum m_BoundingFrustum;
public:
	FrustumCulling() noexcept = default;
	virtual ~FrustumCulling() noexcept = default;
	[[nodiscard]] const bool Initialize(PlayerCamera& playerCamera) noexcept;
	void UpdateFrustum(PlayerCamera& playerCamera) noexcept;
	[[nodiscard]] const std::vector<GameObject*> CullObjects(std::vector<GameObject*>& gameObjects, PlayerCamera& playerCamera) noexcept;
};