#include "ForwardRenderer.h"
//Sets everything up for forward rendering
void ForwardRenderer::BeginFrame() {
	//Clears the color for the scene
	//Set depth buffer
	//Set buffers that will be used in the pipeline on thier respective shader
	//(would propably need an external function that the engine communicates with maybe)
	//Set shaders to forward rendering
}

//Submits what objects that will be rendered
void ForwardRenderer::SubmitObject() {

}

//Cleans up for 2D rendering later
void ForwardRenderer::EndFrame() {
	//Clean the submit pile for the next frame
	//Empty and deselect buffers for the 2D render
}

void ForwardRenderer::CreateVertexShader() {
	//Creates and saves the vertexShader
}

void ForwardRenderer::CreatePixelShader() {
	//Creates and saves the pixelShader
}

//Creates and read in the bare minimum needed for forward rendering
ForwardRenderer::ForwardRenderer() noexcept {
	//Create vertex Shader
	this->CreateVertexShader();
	//Create pixel Shader
	this->CreatePixelShader();
	//Create G-buffer
	//Creates depth and depth-Stencil
}

//Calls private functions to render the frame
void ForwardRenderer::RenderFrame() {
	this->BeginFrame();
	this->EndFrame();
}
