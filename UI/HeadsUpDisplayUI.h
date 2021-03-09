#pragma once
#include "ModuleUI.h"
#include "HeadsUpDisplayBar.h"
#include "..\EventSystem\UIEvents.h"

class HeadsUpDisplayUI : public ModuleUI {
private:
	//General
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pHUDFormat;
	D2D1_RECT_F m_pRightDisplayScreen;
	D2D1_RECT_F m_pLeftDisplayScreen;

	//Distance to planet module
	bool m_pRenderDistance;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pDistanceFormat;
	D2D1_RECT_F m_pDistanceTextBox;
	std::wstring m_pDistanceText;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pPlanetNameFormat;
	D2D1_RECT_F m_pPlanetNameTextBox;
	std::wstring m_pPlanetText;

	//Healthbar
	ID2D1Bitmap* m_pHealthBitmap;
	HeadsUpDisplayBar m_pHealthBar;
	D2D1_RECT_F m_pHealthIcon;

	//OxygenBar
	ID2D1Bitmap* m_pOxygenBitmap;
	HeadsUpDisplayBar m_pOxygenBar;
	D2D1_RECT_F m_pOxygenIcon;

	//FuelBar
	ID2D1Bitmap* m_pFuelBitmap;
	HeadsUpDisplayBar m_pFuelBar;
	D2D1_RECT_F m_pFuelIcon;

	//Weight capacity
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pCapacityFormat;
	ID2D1Bitmap* m_pCapacityBitmap;
	D2D1_RECT_F m_pCapacityTextBox;
	std::wstring m_pCapacityText;
	D2D1_RECT_F m_pCapacityIcon;

	//Warning module
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pWarningTriangle;
	D2D1_RECT_F m_pWarningTextBox;
	std::wstring m_pWarningText;
	bool m_pCapacityWarning;

	//Resources
	std::vector<D2D1_RECT_F> m_pIconPicture;
	std::vector<std::wstring> m_pIconText;

	//Current mouse coords (will be removed once UI event handler is in place)
	unsigned int m_pMouseX;
	unsigned int m_pMouseY;

	//Create modules
	bool CreateDisplayScreens();
	bool CreateBars();
	bool CreateCapacity();
	bool CreateWarningModule();
	bool CreatePlanetDistanceModule();
	bool CreateTools();

	//Update modules if screen size changes
	bool UpdateDisplayScreens();
	bool UpdateBars();
	bool UpdateWarningModule();
	bool UpdatePlanetDistanceModule();
	bool UpdateTools();

	bool UpdateModules();

	//Render modules
	void RenderBars();
	void RenderCapacity();
	void RenderWarningModule();
	void RenderPlanetDistanceModule();

	//For updating things based on information from the event handler
	void SetPlanetDistance(float distanceToPlanet, std::wstring planetName);
	void SetCapacity(unsigned int currentAmount, unsigned int maximumAmount);
public:
	//Creation and destruction functions
	HeadsUpDisplayUI();
	virtual ~HeadsUpDisplayUI() = default;
	bool Initialize();

	//Used by Render2D to render whole module
	void Render();

	void OnEvent(IEvent& event) noexcept;
};

