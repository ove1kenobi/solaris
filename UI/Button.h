#pragma once
#include "ModuleUI.h"

class Button : public ModuleUI {
private:
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
	bool Initialize();

	bool UpdateModules();
	void Render();
};

