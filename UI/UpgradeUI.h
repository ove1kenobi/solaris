#pragma once
#include "ModuleUI.h"

class UpgradeUI : public ModuleUI, public EventPublisher {
private:
	//To identify which upgrade it currently holds
	unsigned int m_pID;
	bool m_pBought;
	bool m_pRenderBitmaps;

	//Colors
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pMainGradientBrush;
	Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> m_pMainGradientStops;
	UINT32 m_pYellow;
	UINT32 m_pWhite;
	UINT32 m_pCyan;
	UINT32 m_pGray;
	UINT32 m_pLightGray;
	UINT32 m_pLightBlue;
	UINT32 m_pDarkblue;

	//Upgrade title
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	D2D1_RECT_F m_pTitleBox;
	std::wstring m_pTitle;

	//Upgrade description
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pUpgradeFormat;
	D2D1_RECT_F m_pHoverBox;
	D2D1_RECT_F m_pTextBox;
	std::wstring m_pDescription;

	//Upgrade cost
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pCostFormat;
	std::list<ID2D1Bitmap*> m_pResourceBitmap;
	std::vector<D2D1_RECT_F> m_pResourcePosition;
	std::vector<D2D1_RECT_F> m_pCostTextbox;
	std::vector<std::wstring> m_pCostText;
	Resources m_pCost;

	//Science requriement
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pScienceFormat;
	ID2D1Bitmap* m_pScienceBitmap;
	D2D1_RECT_F m_pSciencePosition;
	D2D1_RECT_F m_pRequirementTextbox;
	std::wstring m_pRequirement;

	//Create functions
	bool CreateTitle();
	bool CreateDescription();
	bool CreateCost();
	bool CreateTools();

	//Update functions
	bool UpdateModules();

	//Render functions
	void RenderTitle();
	void RenderDescription();
	void RenderCost();

	void InvertCost();
	bool CanAfford(Resources inventory);
public:
	UpgradeUI() noexcept;
	virtual ~UpgradeUI();
	bool Initialize();

	void SetUpgrade(std::wstring upgrade, std::wstring description, unsigned int ID);
	void SetScience(unsigned int science);
	void AddCost(std::wstring resource, std::wstring cost);
	void SetHoverBox(D2D1_RECT_F hoverBox);

	//Main render function
	void Render();
	void OnClick(int mouseX, int mouseY, Resources inventory);

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};
