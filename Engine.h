#pragma once
#include "DXCore.h"
#include "RenderWindow.h"
#include "ForwardRenderer.h"
class Engine
{
private:
	DXCore m_DXCore;
private:
	 
public:
	Engine();
	virtual ~Engine() = default;
	const bool Initialize();
	void Run();
};