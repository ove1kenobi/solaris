#pragma once
#include "Button.h"
#include "..\EventSystem\WindowEvents.h"
#include "../GameEvent.h"
#include "../EventSystem/UIEvents.h"
#include "../EventSystem/SoundEvents.h"

class RandomEventUI : public Button, public EventPublisher {
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
	std::list<ID2D1Bitmap*> m_pIconBitmap;
	std::vector<D2D1_RECT_F> m_pIconPosition;
	std::vector<D2D1_RECT_F> m_pIconTextbox;
	std::vector<std::wstring> m_pIconAmount;

	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pIconTextFormat;
	bool m_pSelected;

	//Sound logic:
	bool m_IsHovering;

	bool CreateText();
	bool CreateDetails();
	bool UpdateDetails();
	bool UpdateTextBoxes();
protected:
public:
	RandomEventUI();
	virtual ~RandomEventUI();
	bool Initialize();

	void AddIcon(std::wstring resource, std::wstring amount);

	bool UpdateModules();
	
	void Render(int mouseX, int mouseY);
	bool OnClick(int mouseX, int mouseY, GameEvent gameEventID);
	void OnEvent(IEvent& event) noexcept;
	void ClearEvent();
	void ClearIcons();
};

