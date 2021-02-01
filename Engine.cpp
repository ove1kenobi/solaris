#include "Engine.h"

Engine::Engine() noexcept
	: m_Running{ true }
{
	m_gameTime.Update();
}

const bool Engine::Initialize()
{
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent);
	//DirectX Core
	if (!m_DXCore.Initialize(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT, m_Window.GetHandle()))
		return false;

	//Forward Renderer:
	if (!m_ForwardRenderer.Initialize(m_DXCore.GetDeviceContext(), m_DXCore.GetBackBuffer(), m_DXCore.GetDepthStencilView()))
		return false;
		
	//Scene
	if (!this->m_scene.init(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT))
		return false;

	DirectX::XMMATRIX WMatrix;
	DirectX::XMMATRIX VMatrix;
	DirectX::XMMATRIX PMatrix;
	this->m_scene.m_gameObjects[0].getWMatrix(WMatrix);
	this->m_scene.m_perspectiveCamera.getVMatrix(VMatrix);
	this->m_scene.m_perspectiveCamera.getPMatrix(PMatrix);
	//Resource Manager
	if (!m_ResourceManager.Initialize(m_DXCore.GetDevice(), m_DXCore.GetDeviceContext(), this->m_scene.m_gameObjects[0].m_vertexBuffer, this->m_scene.m_gameObjects[0].m_indexBuffer, WMatrix, VMatrix, PMatrix))
		return false;

	return true;
}

void Engine::Run()
{
	while (m_Running)
	{
		MSG message = {};
		while (PeekMessageA(&message, nullptr, 0u, 0u, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessageA(&message);				
		}
		//Regular Update:
		Update();

		//Render:
		Render();
	}
}

void Engine::OnEvent(const IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::WindowCloseEvent:
		m_Running = false;
		break;
	}
}

void Engine::Update()
{
	m_gameTime.Update();
	//Here we will update entire scene, though note that the eventsystem is part of doing that for us
}

void Engine::Render()
{
	/*m_ForwardRenderer.BeginFrame() exempelvis */ //Will set up everything and ready rendering pass.
	/*m_ForwardRenderer.Submit(Ett g�ng med cullade modeller...) exempelvis*/
	/*m_ForwardRenderer.EndFrame() exempelvis */

	//Annat tankes�tt �n ovan �r att h�r bara k�ra m_ForwardRenderer.RenderFrame() 
	//och l�ta ovan funktioner vara privata till bara m_ForwardRenderer.
	//this->m_ForwardRenderer.RenderFrame(this->m_DXCore.GetDeviceContext().Get());
	//Followed by 2D-render...

	//Followed by presentation of everything (backbuffer):
	m_ForwardRenderer.RenderFrame(this->m_scene.m_gameObjects[0].m_indexBuffer);
	//HRESULT HR = m_DXCore.GetSwapChain()->Present(1, 0); //TODO: implement 3rd macro for debugging support (Emil F)
	//assert(SUCCEEDED(HR));
	HR_A(m_DXCore.GetSwapChain()->Present(1, 0), "Present");
}
