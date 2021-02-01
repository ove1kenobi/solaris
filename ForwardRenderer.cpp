#include "ForwardRenderer.h"

//NOTE: pointers probably are gonna have to be adjusted to be handled by the functions correctly,
//can also be removed completely based on the structure of the event handler. -steph

ForwardRenderer::ForwardRenderer() noexcept
	: m_Background{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

void ForwardRenderer::OnEvent(IEvent& event) noexcept {
	switch (event.GetEventType()) {
		case EventType::SendRenderObjectsEvent:
			//Do something with objects.
			SendRenderObjectsEvent &derivedEvent = static_cast<SendRenderObjectsEvent&>(event);
			this->m_gameObjects = derivedEvent.getGameObjectVector();
			break;
	}
}


//Sets everything up for forward rendering, takes information from the event handler as input
ID3D11RenderTargetView* ForwardRenderer::BeginFrame(/*ID3D11RenderTargetView* renderTarget*/std::vector<int> indexBuffer)
{
	/*information that is needed from the event handler and resource manager:
	* - what buffers will be used during this render? (Example: WVP and light buffers)
	* - what shaders will be used during this render? (Example: vertex, geometry, pixel)
	* - what renderTarget should we write to? (Will be the one that gets returned as well)
	*/
	//m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), 0u, 1.0f, 0u);
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer.Get(), m_Background);

	AskForRenderObjectsEvent event;
	EventBuss::Get().Delegate(event);
	//Clear the render target and set background here

	//techniques that need to apply before render happens here

	//Set buffers that will be used during this render pass here

	//Set shaders that will be used during this render pass here

	//Return frame for post processing
	m_pDeviceContext->DrawIndexed(m_gameObjects->getIndexBuffer(), 0u, 0u);
	return nullptr;
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
ID3D11RenderTargetView* ForwardRenderer::RenderFrame(std::vector<int> indexBuffer/*ID3D11RenderTargetView* renderTarget, std::vector<GameObject*> gameObjects*/)
{
	//return the finished render to the Engine after applying BeginFrame, SubmitObjects, and then Endframe to it.
	ID3D11RenderTargetView* target = this->BeginFrame(indexBuffer/*renderTarget*/);
	//target = this->SubmitObject(target, gameObjects);
	//return this->EndFrame(target);

	//return this->EndFrame(this->SubmitObject(this->BeginFrame(renderTarget), gameObjects));
	return nullptr;
}
