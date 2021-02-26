#pragma once
#include "CosmicBody.h"

class Orbit;

class Planet : public CosmicBody{
private:
	UINT m_planetType;
public:
	Planet() noexcept;
	virtual ~Planet() = default;

	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
	bool Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo, Orbit* orbit);
};

/*
PLANET TYPES

0 = Hot -> Plasma
1 = Cold -> Khionerite

2 - 5 are normal:
----------------
2 = Radioactive -> Radium
3 = Titanium
4 = Scrap metal
5 = Nanotech
*/