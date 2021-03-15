#pragma once
#include "ModuleUI.h"

class UpgradeUI : public ModuleUI {
private:
	//To identify which upgrade it currently holds
	unsigned int m_pID;

	//Upgrade description
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pUpgradeFormat;
	D2D1_RECT_F m_pHoverBox;
	D2D1_RECT_F m_pTextBox;

	//Upgrade cost
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pCostFormat;
	std::list<ID2D1Bitmap*> m_pResourceBitmap;
	std::vector<D2D1_RECT_F> m_pResourcePosition;
	std::vector<D2D1_RECT_F> m_pCostTextbox;
	std::vector<std::wstring> m_pCost;

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

	void SetUpgrade(std::wstring description, unsigned int ID);
	void AddCost(std::wstring resource, std::wstring cost);
	void SetHoverBox(D2D1_RECT_F hoverBox);

	//Main render function
	void Render();
	void OnClick(int mouseX, int mouseY);

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};
