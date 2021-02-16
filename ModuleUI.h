#pragma once
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem\UtilityEvents.h"

class ModuleUI : public IEventListener {
private:
	void UpdateDXHandlers(IEvent& event) noexcept;
protected:
	//From the event handler
	Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget2D;

	//Refactoring stuff
	Microsoft::WRL::ComPtr<ID2D1GeometrySink> m_pSink;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;

	//Functions
	int GetWidth();
	int GetHeight();
public:
	ModuleUI() noexcept;
	virtual ~ModuleUI() = default;

	bool Initialize();

	void UpdateBrush(D2D1::ColorF color, float opacity);
	void BeginFrame();
	void RenderHelpGrid(int gridSize);
	virtual void RenderUI() = 0;
	void EndFrame();

	void OnEvent(IEvent& event) noexcept;
};

