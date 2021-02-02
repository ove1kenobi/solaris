#include "ForwardRenderer.h"

//NOTE: pointers probably are gonna have to be adjusted to be handled by the functions correctly,
//can also be removed completely based on the structure of the event handler. -steph

ForwardRenderer::ForwardRenderer() noexcept
	: m_Background{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

//Sets everything up for forward rendering, takes information from the event handler as input
ID3D11RenderTargetView* ForwardRenderer::BeginFrame(/*ID3D11RenderTargetView* renderTarget*/)
{
	/* DEMO MODEL (0ve) */
	Model* model = ModelFactory::Get().GetModel(std::string("models/cubes.obj"));

	/*information that is needed from the event handler and resource manager:
	* - what buffers will be used during this render? (Example: WVP and light buffers)
	* - what shaders will be used during this render? (Example: vertex, geometry, pixel)
	* - what renderTarget should we write to? (Will be the one that gets returned as well)
	*/
	//m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), 0u, 1.0f, 0u);
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer.Get(), m_Background);

	//Clear the render target and set background here

	//techniques that need to apply before render happens here

	//Set buffers that will be used during this render pass here

	//Set shaders that will be used during this render pass here

	//Return frame for post processing
	m_pDeviceContext->Draw(model->NumVertices(), 0u);
	return nullptr;
}

//Submits what objects that will be rendered, takes a list of gameObjects as argument
ID3D11RenderTargetView* ForwardRenderer::SubmitObject(ID3D11RenderTargetView* renderTarget, std::vector<GameObject*> gameObjects) {
	//go though the vector and render each object to the renderTarget
	return renderTarget;
}

//Cleans up for the next frame and applies post processing effects
ID3D11RenderTargetView* ForwardRenderer::EndFrame(ID3D11RenderTargetView* renderTarget) {
	/*information that is needed from the event handler and resource manager:
	* - what renderTarget should we write to? (Will be the one that gets returned as well)
	*/

	//Apply the post processing effects to the renderTarget here

	//Return the finished frame
	return renderTarget;
}

const bool ForwardRenderer::Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext,
									   Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pBackBuffer,
									   Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView) noexcept
{
	m_pDeviceContext = pDeviceContext;
	m_pBackBuffer = pBackBuffer;
	m_pDepthStencilView = pDepthStencilView;
#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pDeviceContext && m_pBackBuffer && m_pDepthStencilView);
#endif
	return true;
}

//Calls private functions to render the frame
ID3D11RenderTargetView* ForwardRenderer::RenderFrame(/*ID3D11RenderTargetView* renderTarget, std::vector<GameObject*> gameObjects*/)
{
	//return the finished render to the Engine after applying BeginFrame, SubmitObjects, and then Endframe to it.
	ID3D11RenderTargetView* target = this->BeginFrame(/*renderTarget*/);
	//target = this->SubmitObject(target, gameObjects);
	//return this->EndFrame(target);

	//return this->EndFrame(this->SubmitObject(this->BeginFrame(renderTarget), gameObjects));
	return nullptr;
}
