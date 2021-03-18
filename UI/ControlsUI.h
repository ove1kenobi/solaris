#pragma once
#include "ModuleUI.h"
#include "../EventSystem/SoundEvents.h"

class ControlsUI : public ModuleUI {
private:
	//Colors
	UINT32 m_pGray;
	UINT32 m_pLightGray;
	UINT32 m_pWhite;
	UINT32 m_pHighlight;

	//General
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pTextFont;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTitleFormat;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pBackButtonFormat;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pKeyFormat;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
	bool m_pRenderBitmaps;

	//Screen
	D2D1_RECT_F m_pScreen;
	D2D1_RECT_F m_pMouseDisplay;
	D2D1_RECT_F m_pMovementDisplay;
	D2D1_RECT_F m_pKeyDisplay;
	D2D1_RECT_F m_pControllerTitleBox;
	std::wstring m_pControllerTitle;

	//Back button
	D2D1_RECT_F m_pBackTextBox;
	std::wstring m_pBackText;

	//Press "E" send down probe to planet
	D2D1_ROUNDED_RECT m_pButtonE;
	D2D1_RECT_F m_pButtonBoxE;
	std::wstring m_pButtonTextE;
	D2D1_RECT_F m_pButtonBoxDescriptionE;
	std::wstring m_pButtonTextDescriptionE;

	//Press "Q" ship stabilizer
	D2D1_ROUNDED_RECT m_pButtonQ;
	D2D1_RECT_F m_pButtonBoxQ;
	std::wstring m_pButtonTextQ;
	D2D1_RECT_F m_pButtonBoxDescriptionQ;
	std::wstring m_pButtonTextDescriptionQ;

	//Press "W" accelerate forward
	D2D1_ROUNDED_RECT m_pButtonW;
	D2D1_RECT_F m_pButtonBoxW;
	std::wstring m_pButtonTextW;
	D2D1_RECT_F m_pButtonBoxDescriptionW;
	std::wstring m_pButtonTextDescriptionW;

	//Press "S" accelerate backwards
	D2D1_ROUNDED_RECT m_pButtonS;
	D2D1_RECT_F m_pButtonBoxS;
	std::wstring m_pButtonTextS;
	D2D1_RECT_F m_pButtonBoxDescriptionS;
	std::wstring m_pButtonTextDescriptionS;

	//Press "U" upgrade menu
	D2D1_ROUNDED_RECT m_pButtonU;
	D2D1_RECT_F m_pButtonBoxU;
	std::wstring m_pButtonTextU;
	D2D1_RECT_F m_pButtonBoxDescriptionU;
	std::wstring m_pButtonTextDescriptionU;

	//Press "Left mouse" select event
	ID2D1Bitmap* m_pMouseBitmap;
	D2D1_RECT_F m_pMousePosition;
	D2D1_RECT_F m_pButtonBoxDescriptionLeft;
	std::wstring m_pButtonTextDescriptionLeft;
	D2D1_RECT_F m_pMouseBoxDescription;
	std::wstring m_pMouseTextDescription;

	//Sound logic:
	bool m_IsHoveringBack;

	//Create functions
	bool CreateScreen();
	bool CreateMouse();

	//Update functions
	bool UpdateScreen();
	bool UpdateBack();
	bool UpdateKeys();
	bool UpdateMouse();

	bool UpdateModules();

	//Render functions
	void RenderScreen();
	void RenderBack();
	void RenderKeys();
	void RenderMouse();
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
