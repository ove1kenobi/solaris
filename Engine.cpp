#include "Engine.h"

Engine::Engine() noexcept
	: m_Running{ true }
{

}

const bool Engine::Initialize()
{
	if (!m_DXCore.Initialize(m_Window.GetWidth(), m_Window.GetHeight(), m_Window.GetHandle()))
	{
		return false;
	}
	if (!m_ResourceManager.Initialize(m_DXCore.GetDevice(), m_DXCore.GetDeviceContext()))
	{
		return false;
	}
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
			if (message.message == WM_QUIT)
			{
				m_Running = false;			//Could use future event system to stop running as an event instead (Emil F);	
			}					
		}

		//Regular Update:
		Update();

		//Render:
		Render();
	}
}

void Engine::Update()
{
	//Here we will update entire scene, though note that the eventsystem is part of doing that for us
}

void Engine::Render()
{
	/*m_ForwardRenderer.BeginFrame() exempelvis */ //Will set up everything and ready rendering pass.
	/*m_ForwardRenderer.Submit(Ett g�ng med cullade modeller...) exempelvis*/
	/*m_ForwardRenderer.EndFrame() exempelvis */

	//Annat tankes�tt �n ovan �r att h�r bara k�ra m_ForwardRenderer.RenderFrame() 
	//och l�ta ovan funktioner vara privata till bara m_ForwardRenderer.

	//Followed by 2D-render...


	//Followed by presentation of everything (backbuffer):
	//m_DXCore.GetSwapChain()->Present(0, 0);
}