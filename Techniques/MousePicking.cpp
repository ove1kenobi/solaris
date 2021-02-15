#include "..\pch.h"
#include "MousePicking.h"

MousePicking::MousePicking() noexcept
	: m_pCamera{ nullptr },
	  m_ClientWidth{ 1200u },
	  m_ClientHeight{ 800u },
	  m_pPickedObject{ nullptr },
	  m_DistanceToObject{ 0.0f }
{
	EventBuss::Get().AddListener(this, EventType::DelegateCameraEvent);
}

const bool MousePicking::Initialize() const noexcept
{
	RequestCameraEvent event;
	EventBuss::Get().Get().Delegate(event);
	return true;
}

void MousePicking::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::DelegateCameraEvent:
	{
		DelegateCameraEvent& derivedEvent = static_cast<DelegateCameraEvent&>(event);
		m_pCamera = derivedEvent.GetCamera();
		//If it has received it's matrices, it's content. However, updating the cameras matrices would
		//now mean that the mouse picker would have wrong versions of these.
		EventBuss::Get().RemoveListener(this, EventType::DelegateCameraEvent);
		break;
	}
	}
}
//Note, this function should always in the future take the CULLED objects, and not all objects.
void MousePicking::DoIntersectionTests(const unsigned int& x, const unsigned int& y, const std::vector<GameObject*>& gameObjects) noexcept
{
	DirectX::XMFLOAT4X4 vMatrix = {};
	DirectX::XMFLOAT4X4 pMatrix = {};
	DirectX::XMStoreFloat4x4(&vMatrix, m_pCamera->getVMatrix());
	DirectX::XMStoreFloat4x4(&pMatrix, m_pCamera->getPMatrix());

	//rayViewSpaceDirection is obviously a direction, so w = 0.0f.
	DirectX::XMFLOAT4 rayViewSpaceDirection = {};
	rayViewSpaceDirection.x = (((2.0f * x) / m_ClientWidth) - 1) / pMatrix(0, 0);
	rayViewSpaceDirection.y = (((-2.0f * y) / m_ClientHeight) + 1) / pMatrix(1, 1);
	rayViewSpaceDirection.z = 1.0f;
	rayViewSpaceDirection.w = 0.0f;

	//Origin is a point, so w = 1.0f.
	DirectX::XMFLOAT4 origin = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&vMatrix);
	DirectX::XMVECTOR determinant = DirectX::XMMatrixDeterminant(viewMatrix);
	// 
	if (determinant.m128_f32[0] > 0)
	{
		DirectX::XMMATRIX inverseViewMatrix = DirectX::XMMatrixInverse(&determinant, viewMatrix);

		DirectX::XMVECTOR rayViewSpaceDirectionVector = DirectX::XMLoadFloat4(&rayViewSpaceDirection);
		DirectX::XMVECTOR rayWorldSpaceDirectionVector = DirectX::XMVector4Transform(rayViewSpaceDirectionVector, inverseViewMatrix);
		rayWorldSpaceDirectionVector = DirectX::XMVector4Normalize(rayWorldSpaceDirectionVector);

		DirectX::XMVECTOR originVector = DirectX::XMLoadFloat4(&origin);
		DirectX::XMVECTOR originFinal = DirectX::XMVector4Transform(originVector, inverseViewMatrix);

		float distance = {};
		float t = FLT_MAX;
		m_pPickedObject = nullptr;
		for (unsigned int i{ 0u }; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->IntersectRayObject(originFinal, rayWorldSpaceDirectionVector, distance))
			{
				if (distance < t)
				{
					m_pPickedObject = gameObjects[i];
					t = distance;
					m_DistanceToObject = t;
				}
			}
		}
	}
}

void MousePicking::DisplayPickedObject() noexcept
{
	//Will now display the distance to the picked object from the ORIGIN, which is NOT
	//the center of the ship. However, note that this can be correctly tweaked once the 
	//ship movement is finalized, so a minor revisit is necessary. Also, it doesn't actually
	//differ much as it is, and we are never supposed to be able to get THAT close to a planet! 
	//(Emil F)
	if (m_pPickedObject != nullptr)
	{
		ImGui::Begin("Picked GameObject");
		ImGui::Text("Distance to Object: %.0f", m_DistanceToObject);
		ImGui::End();
	}
	else
	{
		ImGui::Begin("Picked GameObject");
		ImGui::End();
	}
}
