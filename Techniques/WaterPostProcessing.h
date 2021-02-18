#pragma once
#include "..\EventSystem\EventPublisher.h"
#include "..\EventSystem\IEventListener.h"
#include "..\EventSystem\RenderEvents.h"
#include "..\EventSystem\UtilityEvents.h"
#include "..\DXDebug.h"
#include "..\PlayerCamera.h"
#include "..\Planet.h"
struct PlanetData
{
	DirectX::XMVECTOR center[50];
};

struct CameraData
{
	DirectX::XMVECTOR cameraDir;
	DirectX::XMVECTOR cameraPos;
	DirectX::XMMATRIX inverseVMatrix;
	DirectX::XMMATRIX inversePMatrix;
	DirectX::XMMATRIX PMatrix;
};

class WaterPostProcessing : public IEventListener, public EventPublisher
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pPlanetCBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCameraCBuffer;

	PlayerCamera* m_pCamera;
	std::vector<Planet*> m_planets;
private:
	void AssignCamera(IEvent& event) noexcept;
	void AssignPlanets(IEvent& event) noexcept;
	void OnEvent(IEvent& event) noexcept override;
public:
	WaterPostProcessing() noexcept;
	virtual ~WaterPostProcessing() noexcept = default;
	[[nodiscard]] const bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) noexcept;
	void PreparePass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void DoPass(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept;
	void CleanUp() noexcept;
};