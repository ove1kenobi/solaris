#pragma once
#include <functional>
#include "DXCore.h"
#include "RenderWindow.h"
#include "ForwardRenderer.h"
#include "Time.h"
#include "Scene.h"
#include "Resources/ResourceManager.h"
#include "EventSystem\IEventListener.h"
//#include "ModelFactory.h"

class Engine : IEventListener
{
private:
	RenderWindow m_Window;
	DXCore m_DXCore;
	ForwardRenderer m_ForwardRenderer;
	ResourceManager m_ResourceManager;
	bool m_Running;
	Time m_gameTime;
	Scene m_scene;
	long double t_sum;
private:
	void Update();
	void Render();
public:
	Engine() noexcept;
	virtual ~Engine() = default;
	const bool Initialize();
	void Run();
	void OnEvent(IEvent& event) noexcept override;
};