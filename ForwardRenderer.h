#pragma once
#include "GameObject.h"	
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "Bloom.h"

class ForwardRenderer {
private:
	//Color of the screen
	DirectX::XMFLOAT4 m_Background;

	//Techniques that will be applied
	Bloom m_Bloom;

	//Render functions
	ID3D11RenderTargetView* BeginFrame(ID3D11RenderTargetView* renderTarget);
	ID3D11RenderTargetView* SubmitObject(ID3D11RenderTargetView* renderTarget, std::vector<GameObject*> gameObjects);
	ID3D11RenderTargetView* EndFrame(ID3D11RenderTargetView* renderTarget);
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() = default;
	ID3D11RenderTargetView* RenderFrame(ID3D11RenderTargetView* renderTarget, std::vector<GameObject*> gameObjects);
};