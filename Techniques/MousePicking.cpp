#include "..\pch.h"
#include "MousePicking.h"

MousePicking::MousePicking() noexcept
	: m_ClientWidth{ 1200u }, 
	  m_ClientHeight{ 800u }
{
	EventBuss::Get().AddListener(this, EventType::MouseButtonEvent, EventType::DelegateCameraEvent);
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
	case EventType::MouseButtonEvent:
	{
		MouseButtonEvent& derivedEvent = static_cast<MouseButtonEvent&>(event);
		if (derivedEvent.GetKeyState() == KeyState::KeyPress && derivedEvent.GetVirtualKeyCode() == VK_LBUTTON)
			MakeRay(derivedEvent.GetXCoord(), derivedEvent.GetYCoord());
		break;
	}
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

void MousePicking::MakeRay(const unsigned int& x, const unsigned int& y) noexcept
{
	//rayViewSpaceDirection is obviously a direction, so w = 0.0f.
	DirectX::XMFLOAT4X4 vMatrix = {};
	DirectX::XMFLOAT4X4 pMatrix = {};
	DirectX::XMStoreFloat4x4(&vMatrix, m_pCamera->getVMatrix());
	DirectX::XMStoreFloat4x4(&pMatrix, m_pCamera->getPMatrix());
	DirectX::XMFLOAT4 rayViewSpaceDirection = {};
	rayViewSpaceDirection.x = (((2.0f * x) / m_ClientWidth) - 1) / pMatrix(0, 0);
	rayViewSpaceDirection.y = (((-2.0f * y) / m_ClientHeight) + 1) / pMatrix(1,1);
	rayViewSpaceDirection.z = 1.0f;
	rayViewSpaceDirection.w = 0.0f;

	//Origin is a point, so w = 1.0f.
	DirectX::XMFLOAT4 origin = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&vMatrix);
	DirectX::XMVECTOR determinant = DirectX::XMMatrixDeterminant(viewMatrix);
	DirectX::XMMATRIX inverseViewMatrix = DirectX::XMMatrixInverse(&determinant, viewMatrix);
	
	DirectX::XMVECTOR rayViewSpaceDirectionVector = DirectX::XMLoadFloat4(&rayViewSpaceDirection);
	DirectX::XMFLOAT3 rayWorldSpaceDirection = {};
	DirectX::XMStoreFloat3(&rayWorldSpaceDirection, DirectX::XMVector4Transform(rayViewSpaceDirectionVector, inverseViewMatrix));
	
	DirectX::XMVECTOR originVector = DirectX::XMLoadFloat4(&origin);
	DirectX::XMFLOAT3 originFinal = {};
	DirectX::XMStoreFloat3(&originFinal, DirectX::XMVector4Transform(originVector, inverseViewMatrix));
}
