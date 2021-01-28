#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer() noexcept {
	this->inputLayout = nullptr;
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->vertexBuffers = nullptr;
	this->pixelBuffers = nullptr;
}

void ForwardRenderer::SetVertexShader(ID3D11InputLayout* inputLayout, ID3D11VertexShader* vertexShader) {
	this->inputLayout = inputLayout;
	this->vertexShader = vertexShader;
}

void ForwardRenderer::SetPixelShader(ID3D11PixelShader* pixelShader) {
	this->pixelShader = pixelShader;
}

//TODO: fix so they are stored and used correctly
void ForwardRenderer::SetPixelBuffers(int slot, int numberOfBuffers, ID3D11Buffer* pixelBuffers) {
	this->pixelBuffers = pixelBuffers;
}

//TODO: fix so they are stored and used correctly
void ForwardRenderer::SetVertexBuffers(int slot, int numberOfBuffers, ID3D11Buffer* vertexBuffers) {
	this->vertexBuffers = vertexBuffers;
}

//Submits what objects that will be rendered
void ForwardRenderer::SubmitObject() {
	//store objects in a list (either gameObject, Scene, Model, or something else -steph)
}


//Sets everything up for forward rendering
void ForwardRenderer::BeginFrame(ID3D11DeviceContext* gDeviceContext) {
	//Clears the color for the scene
	DirectX::XMFLOAT4 background = {0, 0, 0, 1};

	//Set depth buffer

	//Set buffers that will be used in the pipeline on thier respective shader
	//TODO: fix so they are stored and used correctly
	gDeviceContext->VSSetConstantBuffers(0, 1, &this->vertexBuffers);
	gDeviceContext->PSSetConstantBuffers(0, 1, &this->pixelBuffers);

	//Set shaders
	//TODO: verify that shaders exist before using them
	gDeviceContext->IASetInputLayout(inputLayout);
	gDeviceContext->VSSetShader(vertexShader, nullptr, 0);
	gDeviceContext->PSSetShader(pixelShader, nullptr, 0);

	//render objects from the object list
	//this->objects.Render();
}

//Cleans up gDeviceContext for 2D rendering later
void ForwardRenderer::EndFrame(ID3D11DeviceContext* gDeviceContext) {
	//Clean the submit pile for the next frame
	//Empty and deselect buffers for the 2D render
}

//Calls private functions to render the frame
void ForwardRenderer::RenderFrame(ID3D11DeviceContext* gDeviceContext) {
	this->BeginFrame(gDeviceContext);
	this->EndFrame(gDeviceContext);
}
