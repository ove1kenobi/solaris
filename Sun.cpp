#include "pch.h"
#include "Sun.h"
void Sun::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::RequestSunLightEvent:
	{
		//Answer by sending the light:
		DelegateSunLightEvent event(&m_PointLight);
		EventBuss::Get().Delegate(event);
		break;
	}
	}
}

Sun::Sun() noexcept
{
	EventBuss::Get().AddListener(this, EventType::RequestSunLightEvent);
}

const bool Sun::Initialize() noexcept
{
	//The cosmic body
	this->init(0.0f, 0.0f, 0.0f, 900.0f, 0.0f, 0.0f, 1, nullptr, nullptr);
	//Generate the Sun.
	this->m_model = ModelFactory::Get().GenerateSun(this->m_center.x, this->m_center.y, this->m_center.z, this->m_radius);
	//The light
	//Uses RGB-values as taken from the net from a paper discussing correct sun-RGB-values. Intensity is 1, center is same as sun. 
	m_PointLight.Initialize(DirectX::XMFLOAT3(1.0f, 0.95f, 0.93f), 1.0f, m_center);
	return true;
}

const bool Sun::IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept
{
	return false;
}

const PointLight& Sun::GetPointLight() const
{
	return m_PointLight;
}
