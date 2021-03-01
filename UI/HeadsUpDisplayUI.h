#pragma once
#include "ModuleUI.h"

class HeadsUpDisplayUI : public ModuleUI {
private:
	//General
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pHUDFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pHUDFormat;
	D2D1_RECT_F m_pRightDisplayScreen;
	D2D1_RECT_F m_pLeftDisplayScreen;

	//Crosshair
	float m_pCrosshairSize;
	float m_pCrosshairLength;
	float m_pCrosshairDistance;

	//Distance to planet module
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pDistanceFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pDistanceFormat;
	D2D1_RECT_F m_pDistanceTextBox;
	std::wstring m_pDistanceText;

	//Healthbar - convert later to bar class
	D2D1_RECT_F m_pFullHealthBar;
	D2D1_RECT_F m_pCurrentHealthBar;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pRedGradientBrush;

	//CO2Bar - convert later to bar class
	D2D1_RECT_F m_pFullCO2Bar;
	D2D1_RECT_F m_pCurrentCO2Bar;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pBlueGradientBrush;

	//FuelBar - convert later to bar class
	D2D1_RECT_F m_pFullFuelBar;
	D2D1_RECT_F m_pCurrentFuelBar;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pYellowGradientBrush;

	//Weight capacity
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pCapacityFormat;
	D2D1_RECT_F m_pCapacityTextBox;
	std::wstring m_pCapacityText;

	//Warning module (Will need one for each bar)
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pWarningTriangle;
	D2D1_RECT_F m_pWarningTextBox;
	std::wstring m_pWarningText;

	//MiniMap
	D2D1_RECT_F m_pMiniMap;

	//Resources
	std::vector<D2D1_RECT_F> m_pIconPicture;
	std::vector<std::wstring> m_pIconText;

	//Current mouse coords (will be removed once event handler is in place)
	unsigned int m_pMouseX;
	unsigned int m_pMouseY;

	//Create modules
	bool CreateDisplayScreens();
	bool CreateBars();
	bool CreateCapacity();
	bool CreateWarningModule();
	bool CreateMiniMap();
	bool CreatePlanetDistanceModule();
	bool CreateCrosshair();
	bool CreateTools();

	//Update if screen size changes
	bool UpdateDisplayScreens();
	bool UpdateBars();
	bool UpdateCapacity();
	bool UpdateWarningModule();
	bool UpdateMiniMap();
	bool UpdatePlanetDistanceModule();
	bool UpdateCrosshair();
	bool UpdateTools();

	bool UpdateModules();

	//Render modules
	void RenderRightDisplayScreen();
	void RenderLeftDisplayScreen();
	void RenderBars();
	void RenderCapacity();
	void RenderWarningModule();
	void RenderMiniMap();
	void RenderPlanetDistanceModule();
	void RenderCrosshair();

	//For updating things based on information from the event handler
	void SetPlanetDistance(float distanceToPlanet, std::wstring planetName);
	void SetHealth();
	void SetCO2();
	void SetFuel();
public:
	//Creation and destruction functions
	HeadsUpDisplayUI();
	~HeadsUpDisplayUI();
	bool Initialize();

	//Used by Render2D to render whole module
	void Render();

	void OnEvent(IEvent& event) noexcept;
};

