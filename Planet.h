#pragma once
#include "CosmicBody.h"

class Planet : public CosmicBody{
private:
public:
	Planet() = default;
	~Planet() = default;

	bool Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir);
	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
};