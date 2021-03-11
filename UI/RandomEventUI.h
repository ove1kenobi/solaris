#pragma once
#include "Button.h"
#include "..\EventSystem\WindowEvents.h"

class RandomEventUI : public Button {
private:
	//Hovering details
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pLeftHover;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pRightHover;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomHover;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pHoverTextFormat;
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pHoverFont;
	D2D1_RECT_F m_pHoverTextBox;
	std::wstring m_pHoverText;

	//For icons
	bool m_pDrawBitmaps;
	ID2D1Bitmap* holder;
	std::list<ID2D1Bitmap*> m_pIconBitmap;
	std::vector<D2D1_RECT_F> m_pIconPosition;
	std::vector<D2D1_RECT_F> m_pIconTextbox;
	std::vector<std::wstring> m_pIconAmount;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pIconTextFormat;

	bool CreateText();
	bool CreateDetails();
	bool UpdateDetails();
	bool UpdateTextBoxes();
protected:
public:
	RandomEventUI();
	~RandomEventUI();
	bool Initialize();

	void AddIcon(std::wstring resource, std::wstring amount);

	bool UpdateModules();

	//Should take in coord arguments to figure out what to render
	void Render(int mouseX, int mouseY);
	void OnClick(int mouseX, int mouseY);

	void OnEvent(IEvent& event) noexcept;
};

