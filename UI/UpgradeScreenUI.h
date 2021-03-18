#pragma once
#include "ModuleUI.h"
#include "UpgradeUI.h"
#include "../Player.h"

class UpgradeScreenUI : public ModuleUI {
private:
	//Main screen
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pFormat;
	float m_pDisplayPadding;
	D2D1_RECT_F m_pScreen;
	Resources m_inventory;

	//Colors
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pMainGradientBrush;
	Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> m_pMainGradientStops;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pCornerGradientBrush;
	Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> m_pCornerGradientStops;
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
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pObjectiveFormat;
	D2D1_RECT_F m_pObjectiveTitleBox;
	std::wstring m_pObjectiveTitle;
	D2D1_RECT_F m_pObjectiveDisplayBox;
	D2D1_RECT_F m_pObjectiveTextBox;
	std::wstring m_pObjectiveText;

	//Controllers
	D2D1_RECT_F m_pControllerDisplay;
	D2D1_RECT_F m_pControllerTitleBox;
	std::wstring m_pControllerTitle;

	//Press "E" send down probe to planet
	D2D1_ROUNDED_RECT m_pButtonE;
	D2D1_RECT_F m_pButtonBoxE;
	std::wstring m_pButtonTextE;
	D2D1_RECT_F m_pButtonBoxDescriptionE;
	std::wstring m_pButtonTextDescriptionE;

	//Press "Q" ship stabilizer
	D2D1_ROUNDED_RECT m_pButtonQ;
	D2D1_RECT_F m_pButtonBoxQ;
	std::wstring m_pButtonTextQ;
	D2D1_RECT_F m_pButtonBoxDescriptionQ;
	std::wstring m_pButtonTextDescriptionQ;

	//Press "W" accelerate forward
	D2D1_ROUNDED_RECT m_pButtonW;
	D2D1_RECT_F m_pButtonBoxW;
	std::wstring m_pButtonTextW;
	D2D1_RECT_F m_pButtonBoxDescriptionW;
	std::wstring m_pButtonTextDescriptionW;

	//Press "S" accelerate backwards
	D2D1_ROUNDED_RECT m_pButtonS;
	D2D1_RECT_F m_pButtonBoxS;
	std::wstring m_pButtonTextS;
	D2D1_RECT_F m_pButtonBoxDescriptionS;
	std::wstring m_pButtonTextDescriptionS;

	//Press "U" upgrade menu
	D2D1_ROUNDED_RECT m_pButtonU;
	D2D1_RECT_F m_pButtonBoxU;
	std::wstring m_pButtonTextU;
	D2D1_RECT_F m_pButtonBoxDescriptionU;
	std::wstring m_pButtonTextDescriptionU;

	//Press "Left mouse" select event
	ID2D1Bitmap* m_pMouseBitmap;
	D2D1_RECT_F m_pMousePosition;
	D2D1_RECT_F m_pButtonBoxDescriptionLeft;
	std::wstring m_pButtonTextDescriptionLeft;
	D2D1_RECT_F m_pMouseBoxDescription;
	std::wstring m_pMouseTextDescription;

	//Vector of Resources
	D2D1_RECT_F m_pInventoryTitleBox;
	std::wstring m_pInventoryTitle;
	D2D1_RECT_F m_pResourceDisplay;
	std::vector<std::wstring> resources;
	std::list<ID2D1Bitmap*> m_pResourceBitmap;
	std::vector<D2D1_RECT_F> m_pResourcePosition;
	std::vector<D2D1_RECT_F> m_pAmountTextbox;
	std::vector<D2D1_RECT_F> m_pDescriptionTextbox;
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
	bool CreateTools();

	bool UpdateScreen();
	bool UpdateObjective();
	bool UpdateUpgrades();
	bool UpdateControllerDisplay();
	bool UpdateResourceList();
	bool UpdateTools();

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
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};

