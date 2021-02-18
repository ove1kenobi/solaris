#pragma once
#include "CosmicBody.h"

class Orbit;

class Planet : public CosmicBody{
private:
public:
	Planet() = default;
	virtual ~Planet() = default;


	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
	bool Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo, Orbit* orbit);
};