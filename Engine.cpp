#include "Engine.h"

Engine::Engine() noexcept
	: m_Running{ true }
{
	m_gameTime.Update();
}

const bool Engine::Initialize()
{
	if (!m_DXCore.Initialize(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT, m_Window.GetHandle()))
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

		//Here we will empty our own event queue:
		/*EventHandler.ProcessEvents() or something */

		//Regular Update:
		Update();

		//Render:
		Render();
	}
}

void Engine::Update()
{
	m_gameTime.Update();
}

void Engine::Render()
{
	/*m_ForwardRenderer.BeginFrame() exempelvis */ //Will set up everything and ready rendering pass.
	/*m_ForwardRenderer.Submit(Ett gäng med cullade modeller...) exempelvis*/
	/*m_ForwardRenderer.EndFrame() exempelvis */

	//Annat tankesätt än ovan är att här bara köra m_ForwardRenderer.RenderFrame() 
	//och låta ovan funktioner vara privata till bara m_ForwardRenderer.
	//this->m_ForwardRenderer.RenderFrame(this->m_DXCore.GetDeviceContext().Get());
	//Followed by 2D-render...


	//Followed by presentation of everything (backbuffer):
	//m_DXCore.GetSwapChain()->Present(0, 0);
}