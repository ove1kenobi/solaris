#pragma once
#include "..\EventSystem\WindowEvents.h"
#include "ModuleUI.h"

class MenuUI : public ModuleUI, EventPublisher {
private:
	//General
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pButtonFormat;

	//Colors
	UINT32 m_pWhite;
	UINT32 m_pHighlight;

	//Title
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	D2D1_RECT_F m_pTitleTextBox;
	D2D1_RECT_F m_pTitleTextShadowBox;
	std::wstring m_pTitleText;

	//Start game
	D2D1_RECT_F m_pStartTextBox;
	std::wstring m_pStartText;

	//Exit game
	D2D1_RECT_F m_pExitTextBox;
	std::wstring m_pExitText;

	//Background
	ID2D1Bitmap* m_pBackgroundBitmap;
	D2D1_RECT_F m_pScreen;
	bool m_pRenderBitmaps;

	//Create functions
	bool CreateTitle();
	bool CreateButtons();
	bool CreateScreen();

	//Update functions
	bool UpdateTitle();
	bool UpdateButtons();
	bool UpdateScreen();

	bool UpdateModules();

	//Private render functions
	void RenderTitle();
	void RenderStart();
	void RenderExit();
	void RenderScreen();
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