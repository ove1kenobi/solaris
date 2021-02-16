#pragma once
//#include "ModuleUI.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem\UtilityEvents.h"

class PlanetInteractionUI : public IEventListener {
private:
	//----------------
	//From the event handler
	Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget2D;

	//Refactoring stuff
	Microsoft::WRL::ComPtr<ID2D1GeometrySink> m_pSink;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;

	//Functions
	int GetWidth();
	int GetHeight();
	void UpdateDXHandlers(IEvent& event) noexcept;
	//----------------
	//Main screen
	D2D1_RECT_F m_pMainRectangle;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomLeft;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pBottomRight;
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pTop;

	//Private functions
	bool CreateMainScreen();
	bool CreateTextElements();
	bool CreateDetails();
public:
	PlanetInteractionUI() noexcept;
	virtual ~PlanetInteractionUI() = default;

	bool Initialize();
	void BeginFrame();
	void RenderHelpGrid(int gridSize);
	void RenderUI();
	void EndFrame();

	void OnEvent(IEvent& event) noexcept;
};

