#pragma once
#include "ModuleUI.h"
#include "../EventSystem/SoundEvents.h"

class CreditsUI : public ModuleUI {
private:
	//General
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pButtonFormat;

	//Screen
	D2D1_RECT_F m_pScreen;

	//Back button
	D2D1_RECT_F m_pBackTextBox;
	std::wstring m_pBackText;

	//Sound logic:
	bool m_IsHoveringBack;

	//Create functions
	bool CreateScreen();

	//Update functions
	bool UpdateScreen();
	bool UpdateModules();

	void RenderScreen();
	void RenderBack();
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