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

	//Colors
	UINT32 m_pYellow;
	UINT32 m_pWhite;
	UINT32 m_pCyan;
	UINT32 m_pGray;
	UINT32 m_pLightGray;
	UINT32 m_pLightBlue;
	UINT32 m_pDarkblue;

	//Vector of Upgrades
	std::vector<UpgradeUI*> m_pUpgrades;

	//Objective description
	D2D1_RECT_F m_pObjectiveTextBox;
	std::wstring m_pObjectiveText;

	//Controllers
	ID2D1Bitmap* m_pShipBitmap;
	D2D1_RECT_F m_pShipDisplay;

	//Vector of Resources
	D2D1_RECT_F m_pResourceDisplay;
	std::vector<std::wstring> resources;
	std::list<ID2D1Bitmap*> m_pResourceBitmap;
	std::vector<D2D1_RECT_F> m_pResourcePosition;
	std::vector<D2D1_RECT_F> m_pAmountTextbox;
	std::vector<std::wstring> m_pAmount;

	//Current Science
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pScienceFormat;
	ID2D1Bitmap* m_pScienceBitmap;
	D2D1_RECT_F m_pSciencePosition;
	D2D1_RECT_F m_pRequirementTextbox;
	std::wstring m_pRequirement;
	bool m_pRenderBitmaps;

	bool CreateScreen();
	bool CreateObjective();
	bool CreateUpgrades();
	bool CreateControllerDisplay();
	bool CreateResourceList();

	bool UpdateScreen();
	bool UpdateObjective();
	bool UpdateUpgrades();
	bool UpdateControllerDisplay();
	bool UpdateResourceList();

	bool UpdateModules();

	void RenderScreen();
	void RenderObjective();
	void RenderUpgrades();
	void RenderControllerDisplay();
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

