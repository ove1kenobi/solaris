#pragma once
#include "DXCore.h"
#include "RenderWindow.h"
#include "ForwardRenderer.h"
#include "Time.h"
#include "Scene.h"
#include "ModelFactory.h"

class Engine
{
private:
	RenderWindow m_Window;
	DXCore m_DXCore;
	ForwardRenderer m_ForwardRenderer;
	bool m_Running;
	Time m_gameTime;
	Scene m_scene;
private:
	void Update();
	void Render();
public:
	Engine() noexcept;
	virtual ~Engine() = default;
	const bool Initialize();
	void Run();
};