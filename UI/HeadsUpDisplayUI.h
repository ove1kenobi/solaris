#pragma once
#include "ModuleUI.h"
#include "HeadsUpDisplayBar.h"
#include "..\EventSystem\UIEvents.h"
#include "..\EventSystem\WindowEvents.h"
#include "../Player.h"

class HeadsUpDisplayUI : public ModuleUI {
private:
	//General
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pHUDFormat;
	D2D1_RECT_F m_pRightDisplayScreen;
	D2D1_RECT_F m_pLeftDisplayScreen;
	bool m_pRenderBitmaps;
	Time m_time;

	//Colors
	UINT32 m_pWhite;
	UINT32 m_pYellow;
	UINT32 m_pBlue;
	UINT32 m_pRed;

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

	//Damage warning
	Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> m_pHeatGradientStops;
	Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush> m_pHeatRadialGradientBrush;
	ID2D1Bitmap* m_pFrostBitmap;
	D2D1_RECT_F m_pScreen;

	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pRadiationFormat;
	ID2D1Bitmap* m_pRadiationBitmap;
	D2D1_RECT_F m_pRadiationIcon;
	D2D1_RECT_F m_pRadiationScreen;
	D2D1_RECT_F m_pRadiationTextBox;
	std::wstring m_pRadiationText;

	//Stabilizer
	D2D1_ELLIPSE m_pStabilizerMode;
	D2D1_RECT_F m_pStabilizerTextBox;
	std::wstring m_pStabilizerText;
	bool m_pStabilizer;

	//Velocity
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pVelocityFormat;
	D2D1_RECT_F m_pVelocityTextBox;
	std::wstring m_pVelocityText;
	D2D1_RECT_F m_pUnitTextBox;
	std::wstring m_pUnitText;
	bool m_pMaxVelocity;

	bool m_pRenderCold;
	bool m_pRenderHeat;
	bool m_pRenderRadiation;

	//Resources
	std::vector<D2D1_RECT_F> m_pIconPicture;
	std::vector<std::wstring> m_pIconText;

	//Create modules
	bool CreateDisplayScreens();
	bool CreateBars();
	bool CreateCapacity();
	bool CreateWarningModule();
	bool CreatePlanetDistanceModule();
	bool CreateDamageModules();
	bool CreateStabilizer();
	bool CreateVelocity();
	bool CreateTools();

	//Update modules if screen size changes
	bool UpdateDisplayScreens();
	bool UpdateBars();
	bool UpdateWarningModule();
	bool UpdatePlanetDistanceModule();
	bool UpdateDamageModules();
	bool UpdateStabilizer();
	bool UpdateVelocity();
	bool UpdateTools();

	bool UpdateModules();

	//Render modules
	void RenderBars();
	void RenderCapacity();
	void RenderWarningModule();
	void RenderPlanetDistanceModule();
	void RenderDamageModule();
	void RenderStabilizer();
	void RenderVelocity();

	//For updating things based on information from the event handler
	void SetVelocity();
	void SetPlanetDistance(float distanceToPlanet, std::wstring planetName);
	void SetCapacity(unsigned int currentAmount, unsigned int maximumAmount);
public:
	//Creation and destruction functions
	HeadsUpDisplayUI();
	virtual ~HeadsUpDisplayUI();
	bool Initialize();

	//Used by Render2D to render whole module
	void Render();

	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};

