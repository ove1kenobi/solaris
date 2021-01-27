#pragma once
#include "DXCore.h"
#include "RenderWindow.h"
#include "ForwardRenderer.h"
class Engine
{
private:
	DXCore m_DXCore;
	RenderWindow m_window;
private:
	 
public:
	Engine();
	virtual ~Engine() = default;
	const bool Initialize();
	void Run();
};