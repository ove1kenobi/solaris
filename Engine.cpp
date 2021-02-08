#include "pch.h"
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

	ImGui_ImplWin32_Init(m_Window.GetHandle());
	ModelFactory::Get().setDevice(m_DXCore.GetDevice());

	//Forward Renderer:
	if (!m_ForwardRenderer.Initialize())
		return false;
		
	//Scene
	if (!this->m_scene.init(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT))
		return false;
	
	//Resource Manager
	if (!m_ResourceManager.Initialize())
		return false;

	return true;
}

void Engine::Run()
{
	while (m_Running)
	{
		m_imguiManager.BeginFrame();
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

void Engine::OnEvent(IEvent& event) noexcept
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
	m_scene.update(m_DXCore.GetDeviceContext());

	m_time += m_gameTime.DeltaTime();
	fps++;

	if (m_time >= 1.0f) {
		m_time -= 1.0f;
		m_Window.SetFPS(fps);
		fps = 0;
	}
}

void Engine::Render()
{
	m_ForwardRenderer.RenderFrame();
	m_imguiManager.Render();
	HR_A(m_DXCore.GetSwapChain()->Present(0u, 0u), "Present");
}
