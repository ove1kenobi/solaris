#pragma once
#include "CosmicBody.h"

class Orbit;

class Planet : public CosmicBody{
private:
	std::string m_Tag;
	bool m_TestForCulling;
	float m_DistanceToCamera;
public:
	Planet() noexcept;
	virtual ~Planet() = default;
	[[nodiscard]] const bool IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept override;
	bool Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir, GameObject* tetherTo, Orbit* orbit);
	[[nodiscard]] const std::string& GetTag() const noexcept override;
	[[nodiscard]] const bool& ShallBeTestedForCulling() const noexcept override;
};