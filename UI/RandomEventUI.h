#pragma once
#include "Button.h"

class RandomEventUI : public Button {
private:
	//For hovering
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pLeftHover;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pRightHover;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomHover;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pHoverTextFormat;
	D2D1_RECT_F m_pHoverTextBox;
	std::wstring m_pHoverText;

	//For User Story 30:
	//std::vector<std::wstring> m_pIconPicture;
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

	//For User Story 30:
	void AddIcon(std::wstring amount);

	bool UpdateModules();

	//Should take in coord arguments to figure out what to render
	void Render();
};

/*
What a single random event needs to render:
- a text box where the text will be stored.
- a string of text which represents the event.
- a hover box that informs us when it's time to render hover. (needs to be sent in somehow)
- a text format telling us how the text should look
---------------------------


and in case of hover as well:
- at least 3 or more geometry representing the fancy border
- another text format telling us how the select text should look
*/

