#pragma once
#include "ModuleUI.h"

class PressInteractUI : public ModuleUI {
private:
	//Text
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	D2D1_RECT_F m_pTitleTextBox;
	std::wstring m_pTitleText;

	//Create functions
	bool CreateText();

	//Update functions
	bool UpdateModules();
protected:
public:
	PressInteractUI() noexcept;
	virtual ~PressInteractUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;
};