#pragma once
#include "ModuleUI.h"
#include "Button.h"

class PlanetInteractionUI : public ModuleUI {
private:
	//Main screen
	D2D1_RECT_F m_pMainRectangle;
	Microsoft::WRL::ComPtr< ID2D1LinearGradientBrush> m_pLinearGradientBrush;

	//Corners and details
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeftDetailsOne;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeftDetailsTwo;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRight;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRightDetailsOne;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRightDetailsTwo;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTop;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTopDetailsLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTopDetailsRight;

	//Planet text boxes
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleTextFormat;
	D2D1_RECT_F m_pPlanetNameTextBox;
	std::wstring m_pPlanetNameText;

	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pBodyTextFormat;
	D2D1_RECT_F m_pPlanetFlavourTextBox;
	std::wstring m_pPlanetFlavourText;

	//Random events
	float m_pScreenOffset;
	float m_pPadding;
	float m_pBlockSize;
	std::vector<Button> m_pRandomEvents;

	//------------
	/*
	D2D1_RECT_F m_pEventOneHoverTextBox;
	D2D1_RECT_F m_pEventOneTextBox;
	std::wstring m_pEventOneText;

	D2D1_RECT_F m_pEventTwoHoverTextBox;
	D2D1_RECT_F m_pEventTwoTextBox;
	std::wstring m_pEventTwoText;

	D2D1_RECT_F m_pEventThreeHoverTextBox;
	D2D1_RECT_F m_pEventThreeTextBox;
	std::wstring m_pEventThreeText;

	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pEventHover;
	D2D1_RECT_F m_pEventHoverTextBox;
	std::wstring m_pEventHoverText;
	*/
	//--------------------

	//Creation functions
	bool CreateScreen();
	bool CreateTextElements();
	bool CreateHover();
	bool CreateTools();

	//Update fuctions
	bool UpdateScreen();
	bool UpdateLeftCorner();
	bool UpdateRightCorner();
	bool UpdateTopCorners();
	bool UpdateTools();
	bool UpdateHover();
	bool UpdateModules();

	//Render functions
	void RenderScreen();
	void RenderCorners();
	void RenderPlanetText();
	void RenderRandomEvents();
	void RenderHelpLines();
public:
	PlanetInteractionUI() noexcept;
	virtual ~PlanetInteractionUI() = default;
	bool Initialize();

	void Render();

	//Event functions
	void SetPlanetName(std::wstring text);
	void SetPlanetFlavourText(std::wstring text);
	void OnEvent(IEvent& event) noexcept;
};

