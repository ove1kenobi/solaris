#pragma once

class ForwardRenderer {
private:
	//ID3D11PixelShader* pixelShader;
	//ID3D11VertexShader* vertexShader;
	void BeginFrame();
	void EndFrame();
	void CreateVertexShader();
	void CreatePixelShader();
	//void CreateGbuffer();
	//void CreateDepthStencil();
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() = default;
	void SubmitObject();
	void RenderFrame();
};