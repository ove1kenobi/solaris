#pragma once
#include "ModuleUI.h"
#include "UpgradeUI.h"

class UpgradeScreenUI : public ModuleUI {
private:
	//Main screen
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pFormat;
	D2D1_RECT_F m_pScreen;

	//Vector of Upgrades (Let's presume 6 upgrades for now, but make it possible to change based on amount)
	std::vector<UpgradeUI*> m_pUpgrades;
	//Warp drive
	//Extra cargo space
	//Living quarters
	//Heat protection
	//Cold protection
	//Radiation protection

	//Objective description
	D2D1_RECT_F m_pObjectiveTextBox;
	std::wstring m_pObjectiveText;

	//Bitmap of ship

	//Vector of resources


	bool CreateScreen();
	bool CreateObjective();
	bool CreateUpgrades();
	bool CreateShipDisplay();
	bool CreateResourceList();

	bool UpdateScreen();
	bool UpdateObjective();
	bool UpdateUpgrades();
	bool UpdateShipDisplay();
	bool UpdateResourceList();

	bool UpdateModules();

	void RenderScreen();
	void RenderObjective();
	void RenderUpgrades();
	void RenderShipDisplay();
	void RenderResourceList();
public:
	UpgradeScreenUI() noexcept;
	virtual ~UpgradeScreenUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};

