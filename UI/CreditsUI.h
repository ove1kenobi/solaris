#pragma once
#include "ModuleUI.h"
#include "../EventSystem/SoundEvents.h"

class CreditsUI : public ModuleUI {
private:
	//Colors
	UINT32 m_pGray;
	UINT32 m_pLightGray;
	UINT32 m_pWhite;
	UINT32 m_pHighlight;

	//General
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pButtonFormat;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;

	//Screen
	D2D1_RECT_F m_pScreen;

	//Back button
	D2D1_RECT_F m_pBackTextBox;
	std::wstring m_pBackText;

	//Pictures
	D2D1_RECT_F m_pTitlePictureTextBox;
	std::wstring m_pTitlePictureText;
	D2D1_RECT_F m_pPictureTextBox;
	std::wstring m_pPictureText;

	//Sound
	D2D1_RECT_F m_pTitleSoundTextBox;
	std::wstring m_pTitleSoundText;
	D2D1_RECT_F m_pSoundTextBox;
	std::wstring m_pSoundText;

	//Music
	D2D1_RECT_F m_pTitleMusicTextBox;
	std::wstring m_pTitleMusicText;
	D2D1_RECT_F m_pMusicTextBox;
	std::wstring m_pMusicText;

	//Fonts
	D2D1_RECT_F m_pTitleFontsTextBox;
	std::wstring m_pTitleFontsText;
	D2D1_RECT_F m_pFontsTextBox;
	std::wstring m_pFontsText;

	//Models
	D2D1_RECT_F m_pTitleModelsTextBox;
	std::wstring m_pTitleModelsText;
	D2D1_RECT_F m_pModelsTextBox;
	std::wstring m_pModelsText;

	//Developed by
	D2D1_RECT_F m_pTitleDeveloperTextBox;
	std::wstring m_pTitleDeveloperText;
	D2D1_RECT_F m_pDeveloperTextBox;
	std::wstring m_pDeveloperText;

	//Sound logic:
	bool m_IsHoveringBack;

	//Create functions
	bool CreateScreen();

	//Update functions
	bool UpdateTitles();
	bool UpdateText();
	bool UpdateScreen();
	bool UpdateModules();

	void RenderScreen();
	void RenderTitle();
	void RenderText();
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