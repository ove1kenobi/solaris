#pragma once
#include "DXCore.h"
#include "RenderWindow.h"
#include "ForwardRenderer.h"
#include "Render2D.h"
#include "Time.h"
#include "Scene.h"
#include "Resources/ResourceManager.h" 
#include "EventSystem\IEventListener.h"
#include "EventSystem/UIEvents.h"
#include "ImGui\ImGuiManager.h"
#include "LayerStack.h"
#include "Techniques/MousePicking.h"
#include "SoundManager.h"

class Engine : IEventListener
{
private:
	RenderWindow m_Window;
	DXCore m_DXCore;
	ForwardRenderer m_ForwardRenderer;
	Render2D* m_Render2D;
	ResourceManager m_ResourceManager;
	SoundManager m_SoundManager;
	Time m_gameTime;
	Scene* m_scene;
	ImGuiManager m_imguiManager;
	LayerStack m_LayerStack;
	bool m_Running;
	bool m_MainMenuNotRunning;
	bool m_ResetGame;
	long double m_time;
	int m_fps;
private:
	void RunMainMenu();
	void Update() noexcept;
	void Render();
	void OnEvent(IEvent& event) noexcept override;
public:
	Engine() noexcept;
	virtual ~Engine();
	const bool Initialize();
	void Run();
};