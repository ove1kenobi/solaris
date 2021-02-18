#include "pch.h"
#include "Sun.h"
bool Sun::Initialize() 
{
	//The cosmic body
	this->init(0.0f, 0.0f, 0.0f, 900.0f, 0.0f, 0.0f, 1, nullptr, nullptr);
	//The light
	this->light.Init({ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 100.0f, 200.0f, 300.0f },
		2000.0f);

	//Generate the Sun.
	this->m_model = ModelFactory::Get().GenerateSun(this->m_center.x, this->m_center.y, this->m_center.z, this->m_radius);
	return true;
}

const bool Sun::IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept
{
	return false;
}
