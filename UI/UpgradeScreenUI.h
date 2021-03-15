#pragma once
#include "ModuleUI.h"
#include "UpgradeUI.h"

class UpgradeScreenUI : public ModuleUI {
private:
	//Main screen
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pFormat;
	float m_pDisplayPadding;
	D2D1_RECT_F m_pScreen;

	//Vector of Upgrades
	std::vector<UpgradeUI*> m_pUpgrades;

	//Objective description
	D2D1_RECT_F m_pObjectiveTextBox;
	std::wstring m_pObjectiveText;

	//Bitmap of ship
	ID2D1Bitmap* m_pShipBitmap;
	D2D1_RECT_F m_pShipDisplay;

	//Vector of Resources
	D2D1_RECT_F m_pResourceDisplay;
	std::vector<std::wstring> resources;
	std::list<ID2D1Bitmap*> m_pResourceBitmap;
	std::vector<D2D1_RECT_F> m_pResourcePosition;
	std::vector<D2D1_RECT_F> m_pAmountTextbox;
	std::vector<std::wstring> m_pAmount;

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
	void UpdateAmount();
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};

