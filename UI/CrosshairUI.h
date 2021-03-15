#pragma once
#include "ModuleUI.h"

class CrosshairUI : public ModuleUI {
private:
	float m_pCrosshairDistance;
	float m_pCrosshairLength;
	float m_pCrosshairSize;

	bool UpdateModules();
public:
	CrosshairUI() noexcept;
	virtual ~CrosshairUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};
