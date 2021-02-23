#pragma once
#include "Button.h"

class RandomEventUI : public Button {
private:
	//For hovering
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pLeftHover;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pRightHover;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomHover;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pHoverTextFormat;
	Microsoft::WRL::ComPtr<IDWriteFontCollection> m_pHoverFont;
	D2D1_RECT_F m_pHoverTextBox;
	std::wstring m_pHoverText;

	//For User Story 30:
	//std::vector<> m_pIconPicture;
	std::vector<std::wstring> m_pIconText;
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

	void SetHoverBox(D2D1_RECT_F hoverBox, float textPadding);
	//For User Story 30:
	void AddIcon(std::wstring amount);

	bool UpdateModules();

	//Should take in coord arguments to figure out what to render
	void Render(int mouseX, int mouseY);
	void OnClick(int mouseX, int mouseY);
};

