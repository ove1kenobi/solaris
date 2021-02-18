#include "pch.h"
#include "Engine.h"

Engine::Engine() noexcept
	: m_Running{ true }, m_time{ 0.0 }, m_fps{ 0 }
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
	ModelFactory::Get().setDeviceAndContext(m_DXCore.GetDevice(), m_DXCore.GetDeviceContext());

	//2D Renderer
	if (!m_Render2D.Initialize())
		return false;
	
	//Resource Manager
	if (!m_ResourceManager.Initialize())
		return false;

	//Scene
	if (!this->m_scene.init(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT, m_DXCore.GetDeviceContext()))
		return false;

	//Forward Renderer
	if (!m_ForwardRenderer.Initialize())
		return false;

	//All components must have the correct monitor resolution: (Emil F)
	m_Window.DelegateResolution();

	m_LayerStack.Push(&m_scene);
	m_LayerStack.PushOverlay(&m_imguiManager);
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

void Engine::Update() noexcept
{
	m_gameTime.Update();
	
	m_time += m_gameTime.DeltaTime();
	m_fps++;

	if (m_time >= 1.0f) {
		m_time -= 1.0f;
		m_Window.SetFPS(m_fps);
		m_fps = 0;
	}
	//Should RenderWindow be a layer...? (Emil F)
	m_LayerStack.Update();
	m_Window.Update();
}

void Engine::Render()
{
	m_ForwardRenderer.RenderFrame();
	m_Render2D.RenderUI();

	m_imguiManager.Render();
	HR_A(m_DXCore.GetSwapChain()->Present(0u, 0u), "Present");
}
