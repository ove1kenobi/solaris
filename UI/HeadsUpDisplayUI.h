#pragma once
#include "ModuleUI.h"

class HeadsUpDisplayUI : public ModuleUI {
private:
	//Crosshair
	float m_pCrosshairSize;
	float m_pCrosshairLength;
	float m_pCrosshairDistance;

	//CO2 and health bar
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pRedGradientBrush;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pBlueGradientBrush;
	/*Will need 10 blocks each, can be created with a for loop,
	* but needs to be kept track of so we know which block should be rendered
	will also need 10 numbers*/

	//Weight capacity
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pHUDFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pCapacityFormat;
	//D2D1_RECT_F m_pCapacityTextBox;
	std::wstring m_pCapacityText;

	//Warning modules (Might be better to covert to a class as we will need one for each bar)

	//Distance to planet

	//Resources 10x per resource

	//Minimap

	//Current mouse coords (will be removed once event handler is in place)
	unsigned int m_pMouseX;
	unsigned int m_pMouseY;

	bool CreateCrosshair();
	bool CreateTools();

	bool UpdateCrosshair();
	bool UpdateTools();

	bool UpdateModules();

	void RenderCrosshair();
protected:
public:
	HeadsUpDisplayUI();
	~HeadsUpDisplayUI();
	bool Initialize();

	void Render();

	void OnEvent(IEvent& event) noexcept;
};

