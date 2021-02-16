#pragma once
#include "ModuleUI.h"

class PlanetInteractionUI : ModuleUI {
private:
	//Main screen
	D2D1_RECT_F m_pMainRectangle;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRight;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTop;

	//Text UI elements
	//RandomEventOne
	//RandomEventTwo
	//RandomEventTree
	//PlanetName
	//PlanetFlavourText

	//Extra details

	//Private functions
	bool CreateMainScreen();
	bool CreateTextElements();
	bool CreateDetails();
public:
	PlanetInteractionUI() noexcept;
	virtual ~PlanetInteractionUI() = default;
	[[nodiscard]] const bool Initialize() noexcept;

	void RenderUI();
};

