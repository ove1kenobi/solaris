#pragma once
#include "CosmicBody.h"

class Orbit;

class Planet : public CosmicBody{
private:
	std::string m_Tag;
	std::wstring m_Name;
	bool m_TestForCulling;
	float m_DistanceToCamera;
	UINT m_planetType;
	DirectX::XMFLOAT4 m_WaterColor;
	bool m_VisitedByPlayer;
public:
	Planet() noexcept;
	virtual ~Planet() = default;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept override;
	bool Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo, Orbit* orbit, WaterSphere* waterSphere);
	[[nodiscard]] const std::string& GetTag() const noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
	DirectX::XMFLOAT4 GetWaterColor() noexcept;
	[[nodiscard]] std::wstring& GetName() noexcept;
	[[nodiscard]] const bool& IsVisited() const noexcept;
	void MarkAsVisited() noexcept;
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