#pragma once
#include "ModuleUI.h"
class HeadsUpDisplayBar : public ModuleUI {
private:
	float m_pBarPercentage;
	std::vector<D2D1_RECT_F> m_pFullBar;
	std::vector<D2D1_RECT_F> m_pCurrentBar;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pGradientBrush;

	//Warning module
	//Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pWarningTriangle;
	//D2D1_RECT_F m_pWarningTextBox;
	//std::wstring m_pWarningText;
protected:
public:
	HeadsUpDisplayBar() noexcept;
	virtual ~HeadsUpDisplayBar() = default;
	bool Initialize();

	void SetFullBar(float left, float top, float right, float bottom, float blockPadding);

	bool UpdateModules();
	void Render();

	void OnEvent(IEvent& event) noexcept;
};

