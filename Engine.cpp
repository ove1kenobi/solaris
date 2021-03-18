#include "pch.h"
#include "Engine.h"

Engine::Engine() noexcept
	: m_Running{ true }, m_time{ 0.0 }, m_fps{ 0 }, m_Render2D{ nullptr }, m_scene{ nullptr }, m_MainMenuNotRunning{ false }, m_ResetGame{ false }
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
	EventBuss::Get().AddListener(this, EventType::WindowCloseEvent, EventType::ToggleStartGame, EventType::GameOverEvent);

	m_Render2D = new Render2D();
	m_scene = new Scene();

	//DirectX Core
	if (!m_DXCore.Initialize(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT, m_Window.GetHandle()))
		return false;

	ImGui_ImplWin32_Init(m_Window.GetHandle());
	ModelFactory::Get().setDeviceAndContext(m_DXCore.GetDevice(), m_DXCore.GetDeviceContext());
	
	/* Preload models */
	//const std::string folder = "models/";

	//std::vector<std::string> models;
	//models.insert(models.end(), {
	//	folder + "asteroid-1.obj",
	//	folder + "asteroid-2.obj",
	//	folder + "asteroid-3.obj",
	//	folder + "spaceship_afterburner.obj",
	//	folder + "spaceship_antenna.obj",
	//	folder + "spaceship_cargo.obj",
	//	folder + "spaceship_cold.obj",
	//	folder + "spaceship_fuelcells.obj",
	//	folder + "spaceship_livingquarters.obj",
	//	folder + "spaceship_shield.obj",
	//	folder + "spaceship_warm.obj",
	//	folder + "spaceship_warpdrive.obj"
	//});
	//for (auto m : models)
	//{
	//	ModelFactory::Get().GetModel(m);
	//}
	
	//2D Renderer
	if (!m_Render2D->Initialize())
		return false;
	
	//Resource Manager
	if (!m_ResourceManager.Initialize())
		return false;

	// Sound Manager
	if (!m_SoundManager.Initialize(100.0f, 35.0f))
		return false;

	//Scene
	if (!this->m_scene->init(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT, m_DXCore.GetDeviceContext()))
		return false;

	//Forward Renderer:
	if (!m_ForwardRenderer.Initialize(RenderWindow::DEFAULT_WIN_WIDTH, RenderWindow::DEFAULT_WIN_HEIGHT))
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

		if (m_ResetGame) {
			m_ResetGame = false;
			m_SoundManager.ClearLoopingSounds();
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
	switch (event.GetEventType()) {
	case EventType::WindowCloseEvent:
	{
		m_Running = false;
		m_MainMenuNotRunning = true;
		break;
	}
	case EventType::ToggleStartGame: 
	{
		//Enable player controls
		ToggleControlsEvent myEvent;
		EventBuss::Get().Delegate(myEvent);
		m_MainMenuNotRunning = true;
		break;
	}
	case EventType::GameOverEvent:
	{
		m_ResetGame = true;
		break;
	}
	default:
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
	if (m_MainMenuNotRunning) {
		m_LayerStack.Update();
	}
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
	m_MainMenuNotRunning = false;
	
	while (!m_MainMenuNotRunning) {
		m_imguiManager.BeginFrame();
		MSG message = {};
		while (PeekMessageA(&message, nullptr, 0u, 0u, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
		
		Update();
		m_Render2D->RenderUI();
		m_imguiManager.Render();
		HR_A(m_DXCore.GetSwapChain()->Present(0u, 0u), "Present");
	}

	m_Render2D->ToggleMainMenu();
}