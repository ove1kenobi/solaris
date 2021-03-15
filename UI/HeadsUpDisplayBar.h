#pragma once
#include "ModuleUI.h"
class HeadsUpDisplayBar : public ModuleUI {
private:
	//Block recreation information
	float m_left;
	float m_top;
	float m_right;
	float m_bottom;
	float m_blockSize;
	float m_blockPadding;
	float m_pBarPercentage;

	//vectors containing all blocks
	std::vector<D2D1_RECT_F> m_pFullBar;
	std::vector<D2D1_RECT_F> m_pCurrentBar;

	//colors
	D2D_COLOR_F m_pMainColor;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_pGradientBrush;
	Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> m_pGradientStops;

	//Warning module
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pWarningFormat;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pWarningTriangle;
	D2D1_RECT_F m_pWarningTextBox;
	std::wstring m_pWarningText;

	//Creation functions
	bool CreateWarningModule();
	void ChangeBlockSize(unsigned int block, float amount);
protected:
public:
	HeadsUpDisplayBar() noexcept;
	virtual ~HeadsUpDisplayBar() = default;
	bool Initialize();

	//Functions for the UI class to create and readjust each bar
	void SetTools(D2D_COLOR_F mainColor, D2D_COLOR_F gradientColorLight, D2D_COLOR_F gradientColorDark);
	void SetFullBar(float left, float top, float right, float bottom, float blockPadding);
	void SetCurrentBar(float percentage);

	//Mandatory functions because of ModuleUI
	bool UpdateModules();
	void Render();
	void OnEvent(IEvent& event) noexcept;

	void CleanUp();
};

