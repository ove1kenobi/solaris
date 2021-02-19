#pragma once
#include "ModuleUI.h"

class PlanetInteractionUI : public ModuleUI {
private:
	bool m_pRenderHelpGrids;
	bool m_pRenderRandomEvents;
	enum class RandomEvent {one, two, three, none};

	//Main screen
	D2D1_RECT_F m_pMainRectangle;
	Microsoft::WRL::ComPtr< ID2D1LinearGradientBrush> m_pLinearGradientBrush;

	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeftDetailsOne;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeftDetailsTwo;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRight;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRightDetailsOne;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRightDetailsTwo;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTop;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTopDetailsLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTopDetailsRight;

	//Text boxes
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleTextFormat;
	D2D1_RECT_F m_pPlanetNameTextBox;
	std::wstring m_pPlanetNameText;

	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pBodyTextFormat;
	D2D1_RECT_F m_pPlanetFlavourTextBox;
	std::wstring m_pPlanetFlavourText;

	float m_pBlockSize;

	D2D1_RECT_F m_pEventOneHoverTextBox;
	D2D1_RECT_F m_pEventOneTextBox;
	std::wstring m_pEventOneText;

	D2D1_RECT_F m_pEventTwoHoverTextBox;
	D2D1_RECT_F m_pEventTwoTextBox;
	std::wstring m_pEventTwoText;

	D2D1_RECT_F m_pEventThreeHoverTextBox;
	D2D1_RECT_F m_pEventThreeTextBox;
	std::wstring m_pEventThreeText;

	RandomEvent m_pCurrentHover;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pEventHover;
	D2D1_RECT_F m_pEventHoverTextBox;
	std::wstring m_pEventHoverText;

	//Creation functions
	bool CreateMainScreen();
	bool CreateLeftCorner();
	bool CreateRightCorner();
	bool CreateTopCorners();
	bool CreateTextElements();
	bool CreateHover();
	bool CreateTools();

	bool UpdateModules();
	bool UpdateHover();

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
	void RenderUI();

	//For changing the events once an event system is implimented
	void SetPlanetName(std::wstring text);
	void SetPlanetFlavourText(std::wstring text);
	void SetEventOne(std::wstring text);
	void SetEventTwo(std::wstring text);
	void SetEventThree(std::wstring text);

	void OnEvent(IEvent& event) noexcept;
};

