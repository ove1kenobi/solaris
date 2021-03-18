#pragma once
#include "ModuleUI.h"
#include "..\EventSystem\PlayerEvents.h"
#include "..\EventSystem\SoundEvents.h"

class PauseUI : public ModuleUI, EventPublisher {
private:
	//General
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pButtonFormat;

	//Colors
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pMainGradientBrush;
	Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> m_pMainGradientStops;
	UINT32 m_pWhite;
	UINT32 m_pHighlight;
	UINT32 m_pCyan;
	UINT32 m_pLightBlue;
	UINT32 m_pDarkblue;

	//Title
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	D2D1_RECT_F m_pTitleTextBox;
	D2D1_RECT_F m_pTitleTextShadowBox;
	std::wstring m_pTitleText;

	//Return
	D2D1_RECT_F m_pReturnTextBox;
	std::wstring m_pReturnText;

	//Exit game
	D2D1_RECT_F m_pExitTextBox;
	std::wstring m_pExitText;

	//Background
	D2D1_RECT_F m_pScreen;

	//Sound logics:
	bool m_IsHoveringReturn;
	bool m_IsHoveringExit;

	//Create functions
	bool CreateTitle();
	bool CreateButtons();
	bool CreateTools();

	//Update functions
	bool UpdateTitle();
	bool UpdateButtons();
	bool UpdateScreen();
	bool UpdateTools();

	bool UpdateModules();

	//Private render functions
	void RenderTitle();
	void RenderReturn();
	void RenderExit();
	void RenderScreen();
protected:
public:
	PauseUI() noexcept;
	virtual ~PauseUI();
	bool Initialize();

	//Main render function
	void Render();

	//Event functions
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};
