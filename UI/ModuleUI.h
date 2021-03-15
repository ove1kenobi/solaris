#pragma once
#include "..\EventSystem/IEventListener.h"
#include "..\EventSystem/EventPublisher.h"
#include "..\EventSystem\UtilityEvents.h"
#include "..\EventSystem/InputEvents.h"
#include "..\EventSystem/WindowEvents.h"

//Starting class for all UI elements, contains everything they will need
class ModuleUI : public IEventListener {
protected:
	//For creating UI elements
	Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory2D;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget2D;
	Microsoft::WRL::ComPtr<IDWriteFactory> m_pTextFactory;
	Microsoft::WRL::ComPtr<ID2D1GeometrySink> m_pSink;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;

	//Window information
	float m_pWindowWidth;
	float m_pWindowHeight;

	//Mouse coords information
	unsigned int m_pMouseX;
	unsigned int m_pMouseY;

	//Create functions
	bool CreateBrush();
	std::wstring GetIconFilePath(std::wstring iconFile);
	void LoadBitmapFromFile(PCWSTR filePath , ID2D1Bitmap** bitmap);

	//Updating handlers and error checking
	void UpdateDXHandlers(IEvent& event) noexcept;
	bool ErrorCheck(HRESULT handle, std::string type);

	//For dynamically changing brush without having to create a new one
	void UpdateBrush(UINT32 hexColor, float opacity);

	//Mandatory render functions
	void BeginFrame();
	void EndFrame();
public:
	ModuleUI() noexcept;
	virtual ~ModuleUI();
	virtual bool Initialize() = 0;

	//If screen resolution changes
	virtual bool UpdateModules() = 0;

	//Render functions used by Render2D
	virtual void Render() = 0;
	bool m_pOnScreen;

	//Debugging function
	void RenderHelpGrid(int gridSize);

	virtual void CleanUp() = 0;
};

