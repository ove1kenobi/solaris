#pragma once
#include "ModuleUI.h"
#include "Button.h"

class MenuUI : public ModuleUI {
private:
	//Title
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	D2D1_RECT_F m_pTitleTextBox;
	std::wstring m_pTitleText;

	//Start game
	Button m_pStartGame;

	//Exit game
	Button m_pExitGame;

	//Create functions
	bool CreateTitle();
	bool CreateButtons();

	//Update functions
	bool UpdateTitle();
	bool UpdateButtons();

	bool UpdateModules();
	//Private render functions
protected:
public:
	MenuUI() noexcept;
	virtual ~MenuUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};