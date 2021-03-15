#pragma once
#include "ModuleUI.h"
#include "RandomEventUI.h"
#include "../EventSystem/UtilityEvents.h"
#include "../Player.h"
#include "../GameEvent.h"
//Module handeling everything related to planet interaction UI
class PlanetInteractionUI : public ModuleUI {
private:
	//Main screen and brushes
	D2D1_RECT_F m_pMainRectangle;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pMainGradientBrush;
	Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> m_pMainGradientStops;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pCornerGradientBrush;
	Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> m_pCornerGradientStops;

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
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTitleFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	D2D1_RECT_F m_pPlanetNameTextBox;
	std::wstring m_pPlanetNameText;

	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pBodyFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pBodyFormat;
	D2D1_RECT_F m_pPlanetFlavourTextBox;
	std::wstring m_pPlanetFlavourText;

	//Random events
	float m_pScreenOffset;
	float m_pPadding;
	float m_pBlockSize;
	std::vector<RandomEventUI*> m_pRandomEvents;
	GameEvent m_availableGameEvents[3];
	void SetIcons(int randomEventIndex, Resources resources);

	//Current mouse coords
	unsigned int m_pMouseX;
	unsigned int m_pMouseY;

	//PlayerData:
	PlayerInfo* m_pPlayerInfo;

	//Creation functions
	bool CreateScreen();
	bool CreateTextElements();
	bool CreateTools();

	//Update fuctions
	bool UpdateScreen();
	bool UpdateLeftCorner();
	bool UpdateRightCorner();
	bool UpdateTopCorners();
	bool UpdateTextElements();
	bool UpdateTools();

	bool UpdateModules();

	//Private render functions
	void RenderScreen();
	void RenderCorners();
	void RenderPlanetText();
	void RenderRandomEvents();
public:
	PlanetInteractionUI() noexcept;
	virtual ~PlanetInteractionUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void SetGameEvents(GameEvent gameEvents[3]);
	void SetPlanetName(std::wstring text);
	void SetPlanetFlavourText(std::wstring text);
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();

	bool m_gameEventSelected;
};

