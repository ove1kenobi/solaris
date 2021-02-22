#pragma once
#include "ModuleUI.h"

class Button : public ModuleUI {
private:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
	D2D1_RECT_F m_pHoverBox;
	D2D1_RECT_F m_pTextBox;
	std::wstring m_pText;

	bool CreateButton();
	void RenderHover();
	void RenderClick();
protected:
public:
	Button() noexcept;
	virtual ~Button() = default;
	bool Initialize(D2D1_RECT_F hoverBox, float TextPadding);

	void SetText(std::wstring text);

	bool UpdateModules();
	void Render();

	void OnEvent(IEvent& event) noexcept;
};

