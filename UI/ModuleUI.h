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

	//Create functions
	bool CreateBrush();
	std::wstring GetIconFilePath(std::wstring iconFile);
	void LoadBitmapFromFile(PCWSTR filePath , ID2D1Bitmap** bitmap);

	//Updating handlers and error checking
	void UpdateDXHandlers(IEvent& event) noexcept;
	bool ErrorCheck(HRESULT handle, std::string type);
public:
	ModuleUI() noexcept;
	virtual ~ModuleUI();
	virtual bool Initialize() = 0;

	//For dynamically changing brush without having to create a new one
	void UpdateBrush(D2D1::ColorF color, float opacity);

	//If screen resolution changes
	virtual bool UpdateModules() = 0;

	//Render functions used by Render2D
	void BeginFrame();
	virtual void Render() = 0;
	void EndFrame();

	//Debugging function
	void RenderHelpGrid(int gridSize);
};

