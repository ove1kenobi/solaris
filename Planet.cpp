#include "pch.h"
#include "Planet.h"

Planet::Planet() noexcept
	:m_Tag{ "Planet"},
	 m_TestForCulling{ true },
	 m_DistanceToCamera{ 0.0f },
	 m_planetType{ 0 }
{

}

bool Planet::Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo, Orbit* orbit, WaterSphere* waterSphere) {
	//The cosmic body
	this->init(x, y, z, r, xRot, zRot, rotDir, tetherTo, orbit, waterSphere);
	
	DirectX::XMFLOAT3 yAxis = { m_yAxis.x, m_yAxis.y, m_yAxis.z };

	float distanceFromSun = std::sqrt(x * x + y * y + z * z);
	//Planet types can be found in planet.h
	if (distanceFromSun < 10000.0f)
		m_planetType = 0;
	else if (distanceFromSun > 40000.0f)
		m_planetType = 1;
	else
		m_planetType = type;

	//Generate the Planet.
	this->m_model = ModelFactory::Get().GeneratePlanet(x, y, z, r, m_planetType, yAxis);
	return true;
}

const bool Planet::IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept
{
	DirectX::XMVECTOR Origin = DirectX::XMLoadFloat3(origin);
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(direction);

	return m_model->GetBoundingSphere()->Intersects(Origin, Direction, distance);
}

const std::string& Planet::GetTag() const noexcept
{
	return m_Tag;
}

const bool& Planet::ShallBeTestedForCulling() const noexcept
{
	return m_TestForCulling;
}
