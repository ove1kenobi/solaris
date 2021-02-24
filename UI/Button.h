#pragma once
#include "ModuleUI.h"

//Contains everything a simple button would need, can be inherited for complicated buttons
class Button : public ModuleUI {
protected:
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
	D2D1_RECT_F m_pHoverBox;
	D2D1_RECT_F m_pTextBox;
	std::wstring m_pText;
	float m_pTextPadding;
public:
	Button() noexcept;
	virtual ~Button() = default;
	virtual bool Initialize();

	void SetText(std::wstring text);
	void SetHoverBox(D2D1_RECT_F hoverBox, float textPadding);

	virtual bool UpdateModules();
	void Render();
	virtual void Render(int mouseX, int mouseY);
	virtual void OnClick(int mouseX, int mouseY);

	void OnEvent(IEvent& event) noexcept;
};

