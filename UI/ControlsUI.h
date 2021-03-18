#pragma once
#include "ModuleUI.h"

class ControlsUI : public ModuleUI {
private:
	//Colors

	//Create functions
	bool CreateScreen();

	//Update functions
	bool UpdateScreen();

	bool UpdateModules();

	//Render functions
	void RenderScreen();
protected:
public:
	ControlsUI() noexcept;
	virtual ~ControlsUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};
