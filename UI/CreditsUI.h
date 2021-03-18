#pragma once
#include "ModuleUI.h"

class CreditsUI : public ModuleUI {
private:
	//Text
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pFormat;
	D2D1_RECT_F m_pTextBox;
	std::wstring m_pText;

	//Create functions
	bool CreateScreen();

	//Update functions
	bool UpdateScreen();
	bool UpdateModules();

	void RenderScreen();
protected:
public:
	CreditsUI() noexcept;
	virtual ~CreditsUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};