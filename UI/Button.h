#pragma once
#include "ModuleUI.h"

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

	virtual bool UpdateModules();
	virtual void Render();

	void OnEvent(IEvent& event) noexcept;
};

