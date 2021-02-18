#pragma once
#include "ModuleUI.h"

class PlanetInteractionUI : public ModuleUI {
private:
	bool m_pRenderHelpGrids;
	bool m_pRenderRandomEvents;

	//Main screen
	D2D1_RECT_F m_pMainRectangle;
	Microsoft::WRL::ComPtr< ID2D1LinearGradientBrush> m_pLinearGradientBrush;

	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRight;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTop;

	//Text boxes
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleTextFormat;
	D2D1_RECT_F m_pPlanetNameTextBox;
	std::wstring m_pPlanetNameText;

	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pBodyTextFormat;
	D2D1_RECT_F m_pPlanetFlavourTextBox;
	std::wstring m_pPlanetFlavourText;

	D2D1_RECT_F m_pEventOneTextBox;
	std::wstring m_pEventOneText;

	D2D1_RECT_F m_pEventTwoTextBox;
	std::wstring m_pEventTwoText;

	D2D1_RECT_F m_pEventThreeTextBox;
	std::wstring m_pEventThreeText;

	//Creation functions
	bool CreateMainScreen();
	bool CreateTextElements();
	bool CreateTools();

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
	void RenderUI();

	//For changing the events once an event system is implimented
	void SetPlanetName(std::wstring text);
	void SetPlanetFlavourText(std::wstring text);
	void SetEventOne(std::wstring text);
	void SetEventTwo(std::wstring text);
	void SetEventThree(std::wstring text);

	void OnEvent(IEvent& event) noexcept;
};

/*TODO:
- every module needs these functions:
Initialize() - only get's called once to create everything
update() - only recreates the shapes
render() - render the correct order for each module

*/

