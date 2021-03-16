#include "pch.h"
#include "Asteroid.h"

Asteroid::Asteroid() noexcept
	: m_TestForCulling{ true }, m_deltaPitch{ 0.0f }, m_deltaRoll{ 0.0f }, m_deltaYaw{ 0.0f }, m_ship{ nullptr }, m_IsDestroyed{ false }
{
	m_Tag = "Asteroid";
}

Asteroid::~Asteroid()
{
}

bool Asteroid::init(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 velocity, GameObject* ship)
{
	//Generator and distributions used for generating planet values.
	using t_clock = std::chrono::high_resolution_clock;
	std::default_random_engine gen(static_cast<UINT>(t_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distAst(0, 3);
	std::uniform_real_distribution<float> distScale(0.1f, 1.0f);
	std::uniform_real_distribution<float> distRot(static_cast<float>(-M_PI_2), static_cast<float>(M_PI_2));

	std::vector<std::string> asteroids;
	asteroids.insert(asteroids.end(), {
		"models/Asteroid_1_LOW_MODEL_.obj",
		"models/Asteroid_2_LOW_MODEL_.obj",
		"models/Asteroid_3_LOW_MODEL_.obj",
		"models/Asteroid_4_LOW_MODEL_.obj"
		});

	m_ship = ship;
	m_center = pos;
	m_velocity = velocity;
	m_pitch = distRot(gen);
	m_deltaPitch = distScale(gen);
	m_roll = distRot(gen);
	m_deltaRoll = distScale(gen);
	m_yaw = distRot(gen);
	m_deltaYaw = distScale(gen);
	//m_model = ModelFactory::Get().GetModel(asteroids[distAst(gen)]);
	// Stand-in for asteroids during testing - comment 'Preload models' in Engine::Initialize()
	m_model = ModelFactory::Get().GetModel("models/cubemetal.obj");
	m_scale = distScale(gen) * 30.0f;
	if ((m_scale / 30) <= 0.2)
	{
		m_boundingSphere.Radius *= m_scale * 24;
	}
	else if ((m_scale / 30) <= 0.4)
	{
		m_boundingSphere.Radius *= m_scale * 18;
	}
	else if ((m_scale / 30) <= 0.6)
	{
		m_boundingSphere.Radius *= m_scale * 15;
	}
	else if ((m_scale / 30) <= 0.7)
	{
		m_boundingSphere.Radius *= m_scale * 12;
	}
	else if ((m_scale / 30) <= 0.85)
	{
		m_boundingSphere.Radius *= m_scale * 9;
	}
	else
	{
		m_boundingSphere.Radius *= m_scale * 7;
	}
	m_mass = m_model->GetBoundingSphere()->Radius * 50000.0f * m_scale;
	ModelFactory::Get().CreateMatrixBuffer(m_AmatrixBuffer);
	return true;
}

GameObject* Asteroid::update(DirectX::XMFLOAT4X4 VMatrix, DirectX::XMFLOAT4X4 PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	if (m_IsDestroyed)
	{
		return this;
	}
	float distShip = length(m_center - m_ship->GetCenter());
	float distSun = length(m_center);
	if (distShip > 20000 || distSun < 500.0f) return this;
	m_pitch += static_cast<float>(m_deltaPitch * m_timer.DeltaTime());
	m_roll += static_cast<float>(m_deltaRoll * m_timer.DeltaTime());
	m_yaw += static_cast<float>(m_deltaYaw * m_timer.DeltaTime());
	UpdatePhysics();
	//Updated the same way as a cosmicbody, with S * R * T. Rotation is around the ships up vector.
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_upVector);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_scale, m_scale, m_scale);
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYaw(m_roll, m_pitch, m_yaw);
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);
	DirectX::XMMATRIX result = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&m_wMatrix, DirectX::XMMatrixTranspose(result));

	m_boundingSphere.Center = m_center;

	//Update the matrixBuffer.

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	DirectX::XMFLOAT4X4 wvpMatrix;
	DirectX::XMMATRIX vMatrix = DirectX::XMLoadFloat4x4(&VMatrix);
	DirectX::XMMATRIX pMatrix = DirectX::XMLoadFloat4x4(&PMatrix);

	DirectX::XMStoreFloat4x4(&wvpMatrix, DirectX::XMMatrixTranspose(result * vMatrix * pMatrix));

	HR_X(deviceContext->Map(
		m_AmatrixBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource), "Map");

	memcpy(mappedSubresource.pData, &m_wMatrix, sizeof(DirectX::XMFLOAT4X4));
	memcpy(((char*)mappedSubresource.pData)+ sizeof(DirectX::XMFLOAT4X4), &wvpMatrix, sizeof(DirectX::XMFLOAT4X4));

	deviceContext->Unmap(m_AmatrixBuffer.Get(), 0);
	return nullptr;
}

void Asteroid::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	for (auto tex : m_model->GetTextures())
	{
		if (tex) tex->Bind(deviceContext);
	}
	deviceContext->IASetVertexBuffers(0u,
		1u,
		m_model->getVertexBuffer().GetAddressOf(),
		&m_model->getStride(),
		&m_model->getOffset());
	deviceContext->IASetIndexBuffer(m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->VSSetConstantBuffers(0, 1, m_AmatrixBuffer.GetAddressOf());
}

const bool Asteroid::IntersectRayObject(const DirectX::XMFLOAT3* origin, const DirectX::XMFLOAT3* direction, float& distance) noexcept
{
	return false;
}

const bool& Asteroid::ShallBeTestedForCulling() const noexcept
{
	return m_TestForCulling;
}

void Asteroid::BindShadowUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
	pDeviceContext->IASetVertexBuffers(0u,
		1u,
		this->m_model->getVertexBuffer().GetAddressOf(),
		&this->m_model->getStride(),
		&this->m_model->getOffset());
	pDeviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Asteroid::MarkAsDestroyed() noexcept
{
	m_IsDestroyed = true;
}

void Asteroid::SetVelocity(const DirectX::XMFLOAT3& velocity) noexcept
{
	m_velocity = velocity;
}
