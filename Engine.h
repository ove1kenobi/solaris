#pragma once
#include "DXCore.h"
#include "RenderWindow.h"
#include "ForwardRenderer.h"
#include "Render2D.h"
#include "Time.h"
#include "Scene.h"
#include "Resources/ResourceManager.h" 
#include "EventSystem\IEventListener.h"
#include "ImGui\ImGuiManager.h"
#include "LayerStack.h"

class Engine : IEventListener
{
private:
	RenderWindow m_Window;
	DXCore m_DXCore;
	ForwardRenderer m_ForwardRenderer;
	Render2D m_Render2D;
	ResourceManager m_ResourceManager;
	Time m_gameTime;
	Scene m_scene;
	ImGuiManager m_imguiManager;
	LayerStack m_LayerStack;
	bool m_Running;
	long double m_time;
	int fps;
private:
	void Update();
	void Render();
	void OnEvent(IEvent& event) noexcept override;
public:
	Engine() noexcept;
	virtual ~Engine() = default;
	const bool Initialize();
	void Run();
};