#include "pch.h"
#include "Planet.h"

Planet::Planet(const std::wstring& name) noexcept
	:m_Name{ name },
	 m_TestForCulling{ true },
	 m_DistanceToCamera{ 0.0f },
	 m_planetType{ 0 },
	 m_WaterColor{ 0.0f, 0.0f, 1.0f, 1.0f },
	 m_VisitedByPlayer{ false }
{
	m_Tag = "Planet";
}

bool Planet::Initialize(float x, float y, float z, float r, float xRot, float zRot, int rotDir, UINT type, GameObject* tetherTo, Orbit* orbit, WaterSphere* waterSphere) {
	//The cosmic body
	this->init(x, y, z, r, xRot, zRot, rotDir, tetherTo, orbit, waterSphere);
	
	DirectX::XMFLOAT3 yAxis = { m_yAxis.x, m_yAxis.y, m_yAxis.z };

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

	m_boundingSphere.Radius = r;
	return true;
}

const bool Planet::IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept
{
	DirectX::XMVECTOR Origin = DirectX::XMLoadFloat3(origin);
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(direction);

	return m_model->GetBoundingSphere()->Intersects(Origin, Direction, distance);
}

const bool& Planet::ShallBeTestedForCulling() const noexcept
{
	return m_TestForCulling;
}

DirectX::XMFLOAT4 Planet::GetWaterColor() noexcept
{
	return m_WaterColor;
}

std::wstring& Planet::GetName() noexcept
{
	return m_Name;
}

const bool& Planet::IsVisited() const noexcept
{
	return m_VisitedByPlayer;
}

void Planet::MarkAsVisited() noexcept
{
	m_VisitedByPlayer = true;
}
