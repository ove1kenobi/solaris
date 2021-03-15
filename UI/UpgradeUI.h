#pragma once
#include "ModuleUI.h"

class UpgradeUI : public ModuleUI {
private:
	//Upgrade description
	D2D1_RECT_F m_pHoverBox;
	//Upgrade cost

	//Create functions
	bool CreateDescription();
	bool CreateCost();

	//Update functions
	bool UpdateDescription();
	bool UpdateCost();

	bool UpdateModules();

	//Render functions
	void RenderDescription();
	void RenderCost();
public:
	UpgradeUI() noexcept;
	virtual ~UpgradeUI();
	bool Initialize();

	void SetHoverBox(D2D1_RECT_F hoverBox);

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};
