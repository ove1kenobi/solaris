#include "pch.h"
#include "Engine.h"

Engine::Engine() noexcept
	: m_Running{ true }, m_time{ 0.0 }, m_fps{ 0 }, m_Render2D{ nullptr }, m_scene{ nullptr }
{
	m_gameTime.Update();
}

Engine::~Engine()
{
	delete m_scene;
	delete m_Render2D;
}

const bool Engine::Initialize()
{
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent);

	m_Render2D = new Render2D();
	m_scene = new Scene();

	//DirectX Core
	if (!m_DXCore.Initialize(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT, m_Window.GetHandle()))
		return false;

	ImGui_ImplWin32_Init(m_Window.GetHandle());
	ModelFactory::Get().setDeviceAndContext(m_DXCore.GetDevice(), m_DXCore.GetDeviceContext());
	
	/* Preload models */
	
	/*std::vector<std::string> models;
	models.insert(models.end(), {
		"models/Asteroid_1_LOW_MODEL_.obj",
		"models/Asteroid_2_LOW_MODEL_.obj",
		"models/Asteroid_3_LOW_MODEL_.obj",
		"models/Asteroid_4_LOW_MODEL_.obj"
		});
	for (auto m : models)
	{
		ModelFactory::Get().GetModel(m);
	}*/
	
	//2D Renderer
	if (!m_Render2D->Initialize())
		return false;
	
	//Resource Manager
	if (!m_ResourceManager.Initialize())
		return false;

	//Scene
	if (!this->m_scene->init(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT, m_DXCore.GetDeviceContext()))
		return false;

	//Forward Renderer:
	if (!m_ForwardRenderer.Initialize(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT))
		return false;

	// Sound Manager
	if (!m_SoundManager.Initialize())
		return false;

	//All components must have the correct monitor resolution: (Emil F)
	m_Window.DelegateResolution();

	m_LayerStack.Push(m_scene);
	m_LayerStack.PushOverlay(&m_imguiManager);

	//Runs until the game is started.
	RunMainMenu();
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

		//The player died, restart the scene.
		if (m_scene->GetPlayerHealth() <= 0) {
			//Deletes the scene aswell
			m_LayerStack.RemoveFirst();
			
			m_Render2D->CleanUp();
			delete m_Render2D;

			m_Render2D = new Render2D;
			m_scene = new Scene();
			m_LayerStack.Push(m_scene);

			m_DXCore.DelegateDXHandles();

			if (!m_Render2D->Initialize())
				assert(false);

			if (!this->m_scene->init(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT, m_DXCore.GetDeviceContext()))
				assert(false);

			m_Window.DelegateResolution();

			RunMainMenu();
		}
	}
}

void Engine::OnEvent(IEvent& event) noexcept
{
	switch (event.GetEventType())
	{
	case EventType::WindowCloseEvent:
	{
		m_Running = false;
		break;
	}
	case EventType::KeyboardEvent:
	{
		int virKey = static_cast<KeyboardEvent*>(&event)->GetVirtualKeyCode();
		if (virKey == 'V') {
			m_MainMenuRunning = false;
		}
	}
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
	m_SoundManager.Update();
}

void Engine::Render()
{
	m_ForwardRenderer.RenderFrame();
	m_Render2D->RenderUI();

	m_imguiManager.Render();
	HR_A(m_DXCore.GetSwapChain()->Present(0u, 0u), "Present");
}

void Engine::RunMainMenu() {
	m_MainMenuRunning = true;
	//While button to start the game is not pressed (v atm). False for now until RenderMainMenu is implemented.
	while (/*m_MainMenuRunning*/ false) {
		//Render the main menu.
		//m_Render2D->RenderMainMenu();
	}
}