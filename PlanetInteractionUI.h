#pragma once
#include "ModuleUI.h"

class PlanetInteractionUI : public ModuleUI {
private:
	//Main screen
	D2D1_RECT_F m_pMainRectangle;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRight;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTop;

	//Private functions
	bool CreateMainScreen();
	bool CreateTextElements();
	bool CreateDetails();
public:
	PlanetInteractionUI() noexcept;
	virtual ~PlanetInteractionUI() = default;

	bool Initialize();
	void RenderUI();
};

