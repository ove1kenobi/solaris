#pragma once
#include <d3d11.h>
//#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Bloom.h"

class ForwardRenderer {
private:
	DirectX::XMFLOAT4 m_Background;
	Bloom m_Bloom;
	//std::vector<GameObjects> m_GameObjects;
	ID3D11RenderTargetView* BeginFrame(ID3D11RenderTargetView* renderTarget);
	ID3D11RenderTargetView* EndFrame(ID3D11RenderTargetView* renderTarget);
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() = default;

	void SubmitObject();
	ID3D11RenderTargetView* RenderFrame(ID3D11RenderTargetView* renderTarget);
};