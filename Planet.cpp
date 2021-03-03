#include "pch.h"
#include "Planet.h"

Planet::Planet() noexcept
	:m_Tag{ "Planet"},
	 m_TestForCulling{ true },
	 m_DistanceToCamera{ 0.0f },
	 m_planetType{ 0 },
	 m_WaterColor{ 0.0f, 0.0f, 1.0f, 1.0f }
{

}

bool Planet::Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo, Orbit* orbit, WaterSphere* waterSphere) {
	//The cosmic body
	this->init(x, y, z, r, xRot, zRot, rotDir, tetherTo, orbit, waterSphere);
	
	DirectX::XMFLOAT3 yAxis = { m_yAxis.x, m_yAxis.y, m_yAxis.z };

	float distanceFromSun = std::sqrt(x * x + y * y + z * z);
	//Planet types can be found in planet.h
	if (distanceFromSun < 5000.0f)
		m_planetType = 0;
	else if (distanceFromSun > 10000.0f)
		m_planetType = 1;
	else
		m_planetType = type;

	switch (m_planetType) {
	case 0:
		m_WaterColor = { 1.0f, 0.6f, 0.0f, 1.0f };
		break;
	case 1:
		m_WaterColor = { 0.863f, 0.953f, 1.0f, 1.0f };
		break;
	case 2:
		m_WaterColor = { 0.978f, 0.39f, 0.992f, 1.0f };
		break;
	case 3:
		m_WaterColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		break;
	case 4:
		m_WaterColor = { 0.0f, 0.0f, 1.0f, 1.0f };
		break;
	case 5:
		m_WaterColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		break;
	}

	//Generate the Planet.
	this->m_model = ModelFactory::Get().GeneratePlanet(x, y, z, r, m_planetType, yAxis, m_WaterColor);
	return true;
}

const bool Planet::IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept
{
	return m_model->GetBoundingSphere()->Intersects(origin, direction, distance);
}

const std::string& Planet::GetTag() const noexcept
{
	return m_Tag;
}

const bool& Planet::ShallBeTestedForCulling() const noexcept
{
	return m_TestForCulling;
}

DirectX::XMFLOAT4 Planet::GetWaterColor() noexcept
{
	return m_WaterColor;
}