#include "pch.h"
#include "Asteroid.h"

Asteroid::Asteroid() noexcept
	: m_scale { 1.0f }
{

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
	m_model = ModelFactory::Get().GetModel(asteroids[distAst(gen)]);
	m_scale = distScale(gen) * 30.0f;
	m_mass = length(m_model->GetBoundingBox()->Extents) * 50000.0f * m_scale;
	ModelFactory::Get().CreateMatrixBuffer(m_AmatrixBuffer);
	return true;
}

GameObject* Asteroid::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
{
	float distance = length(m_center - m_ship->GetCenter());
	if (distance > 20000) return this;
	m_pitch += static_cast<float>(m_deltaPitch * m_timer.DeltaTime());
	m_roll += static_cast<float>(m_deltaRoll * m_timer.DeltaTime());
	m_yaw += static_cast<float>(m_deltaYaw * m_timer.DeltaTime());
	UpdatePhysics();
	//Updated the same way as a cosmicbody, with S * R * T. Rotation is around the ships up vector.
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_upVector);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_scale, m_scale, m_scale);
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYaw(m_roll, m_pitch, m_yaw);
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);
	DirectX::XMMATRIX final = scale * rot * trans;
	DirectX::XMStoreFloat4x4(&m_wMatrix, final);

	//Update the matrixBuffer.

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ModelFactory::MatrixBuffer* data;

	DirectX::XMMATRIX WMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_wMatrix));
	VMatrix = DirectX::XMMatrixTranspose(VMatrix);
	PMatrix = DirectX::XMMatrixTranspose(PMatrix);

	deviceContext->Map(
		m_AmatrixBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource
	);

	data = (ModelFactory::MatrixBuffer*)mappedSubresource.pData;

	data->WMatrix = WMatrix;
	data->VMatrix = VMatrix;
	data->PMatrix = PMatrix;

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

const bool Asteroid::IntersectRayObject(const DirectX::FXMVECTOR& origin, const DirectX::FXMVECTOR& direction, float& distance) noexcept
{
	DirectX::BoundingBox b = *(m_model->GetBoundingBox());
	b.Center = m_center;
	return b.Intersects(origin, direction, distance);
}