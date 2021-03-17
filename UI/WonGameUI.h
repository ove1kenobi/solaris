#pragma once
#include "ModuleUI.h"

class WonGameUI : public ModuleUI {
private:
	//Text
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pFormat;
	D2D1_RECT_F m_pTextBox;
	std::wstring m_pText;

	//Create functions
	bool CreateText();

	//Update functions
	bool UpdateModules();
protected:
public:
	WonGameUI() noexcept;
	virtual ~WonGameUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};