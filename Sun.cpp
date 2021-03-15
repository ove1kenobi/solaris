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
	case EventType::RequestSunEvent:
	{
		DelegateSunEvent event(&m_center, &m_radius);
		EventBuss::Get().Delegate(event);
		break;
	}
	}
}

Sun::Sun() noexcept
	: m_TestForCulling{ true }
{
	EventBuss::Get().AddListener(this, EventType::RequestSunLightEvent, EventType::RequestSunEvent);
	m_Tag = "Sun";
}

Sun::~Sun() {
	EventBuss::Get().RemoveListener(this, EventType::RequestSunLightEvent);
	EventBuss::Get().RemoveListener(this, EventType::RequestSunEvent);
}
const bool Sun::Initialize() noexcept
{
	float radius = 900.0f;
	m_scale = radius;
	//The cosmic body
	this->init(0.0f, 0.0f, 0.0f, radius, 0.0f, 0.0f, 1, nullptr, nullptr, nullptr);
	//Generate the Sun.
	//this->m_model = ModelFactory::Get().GenerateSun(this->m_center.x, this->m_center.y, this->m_center.z, this->m_radius);
	this->m_model = ModelFactory::Get().GetModel("models/Sun.obj");
	//The light
	//Uses RGB-values as taken from the net from a paper discussing correct sun-RGB-values. Intensity is 1, center is same as sun. 
	m_PointLight.Initialize(DirectX::XMFLOAT3(1.0f, 0.95f, 0.93f), 1.0f, m_center);
	m_HasBoundingVolume = true;
	m_boundingSphere.Radius = radius;
	m_mass *= 1.0e3f;
	return true;
}

void Sun::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {
	for (auto tex : m_model->GetTextures())
	{
		if (tex) tex->Bind(deviceContext);
	}

	deviceContext->IASetVertexBuffers(0u,
		1u,
		this->m_model->getVertexBuffer().GetAddressOf(),
		&this->m_model->getStride(),
		&this->m_model->getOffset());
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0u, 1u, this->m_model->getMatrixBuffer().GetAddressOf());
}

const bool Sun::IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept
{
	return false;
}

const PointLight& Sun::GetPointLight() const
{
	return m_PointLight;
}

const bool& Sun::ShallBeTestedForCulling() const noexcept
{
	return m_TestForCulling;
}
