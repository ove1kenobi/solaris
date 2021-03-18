#pragma once
#include "ModuleUI.h"

class ControlsUI : public ModuleUI {
private:
	//Colors

	//General
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pButtonFormat;

	//Screen
	D2D1_RECT_F m_pScreen;

	//Back button
	D2D1_RECT_F m_pBackTextBox;
	std::wstring m_pBackText;

	//Create functions
	bool CreateScreen();

	//Update functions
	bool UpdateScreen();

	bool UpdateModules();

	//Render functions
	void RenderScreen();
	void RenderBack();
protected:
public:
	ControlsUI() noexcept;
	virtual ~ControlsUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};
