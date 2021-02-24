#pragma once
#include "ModuleUI.h"

class HeadsUpDisplayUI : public ModuleUI {
private:
	/*Modules the HUD needs:
	* - a CO2 bar (seperate class)
	* - a hull/health bar (seperate class)
	* - weight capacity number display
	* - planet distance number display
	* - 10x resource picture display (vector, will be reused for UpgradesUI as well)
	* - 10x resource amount number display (vector, will be reused for UpgradesUI as well)
	* - minimap picture display
	* - Hitcross shapes
	*/
protected:
public:
	HeadsUpDisplayUI();
	~HeadsUpDisplayUI();
	bool Initialize();

	bool UpdateModules();

	void Render();

	void OnEvent(IEvent& event) noexcept;
};

