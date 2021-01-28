#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class ForwardRenderer {
private:
	//Refrence to shaders that will be used in the pipeline
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	//Refrence to buffers which will be used in each shader stage
	ID3D11Buffer* vertexBuffers;
	ID3D11Buffer* pixelBuffers;

	//List of objects which need to render

	//Render and clean up functions
	void BeginFrame(ID3D11DeviceContext* gDeviceContext);
	void EndFrame(ID3D11DeviceContext* gDeviceContext);
public:
	ForwardRenderer() noexcept;
	virtual ~ForwardRenderer() = default;

	//Will in the future take render objects as argument and store them for render
	void SubmitObject();
	void RenderFrame(ID3D11DeviceContext* gDeviceContext);

	//Functions for configuring Forward Render
	void SetVertexShader(ID3D11InputLayout* inputLayout, ID3D11VertexShader* vertexShader);
	void SetPixelShader(ID3D11PixelShader* pixelShader);
	void SetVertexBuffers(int slot, int numberOfBuffers, ID3D11Buffer* ppConstantBuffers);
	void SetPixelBuffers(int slot, int numberOfBuffers, ID3D11Buffer* ppConstantBuffers);
};