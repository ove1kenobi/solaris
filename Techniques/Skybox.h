#pragma once
#include "..\EventSystem\EventPublisher.h"
#include "..\EventSystem\IEventListener.h"
#include "..\EventSystem\RenderEvents.h"
#include "..\EventSystem\UtilityEvents.h"
#include "..\DXDebug.h"
#include "..\PlayerCamera.h"
struct MatrixBuffer 
{
	DirectX::XMMATRIX VMatrix;
	DirectX::XMMATRIX PMatrix;
};

class Skybox : public IEventListener, public EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCameraCBuffer;
	Camera* m_pCamera;
private:
	void AssignCamera(IEvent& event) noexcept;
	void OnEvent(IEvent& event) noexcept override;
public:
	Skybox() noexcept;
	virtual ~Skybox() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) noexcept;
	void PreparePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void DoPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void CleanUp() noexcept;
};