#pragma once
#include "GameObject.h"	
#include "DXDebug.h"
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "Bloom.h"

class ForwardRenderer {
private:
	//Color of the screen
	FLOAT m_Background[4];

	//Techniques that will be applied
	Bloom m_Bloom;

	//Emil F:s edits:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	//Render functions
	ID3D11RenderTargetView* BeginFrame(/*ID3D11RenderTargetView* renderTarget*/std::vector<int> indexBuffer);
	ID3D11RenderTargetView* SubmitObject(ID3D11RenderTargetView* renderTarget, std::vector<GameObject*> gameObjects);
	ID3D11RenderTargetView* EndFrame(ID3D11RenderTargetView* renderTarget);
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() = default;
	[[nodiscard]] const bool Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext, 
										Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pBackBuffer, 
										Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView) noexcept;
	ID3D11RenderTargetView* RenderFrame(/*ID3D11RenderTargetView* renderTarget, std::vector<GameObject*> gameObjects*/std::vector<int> indexBuffer);
};