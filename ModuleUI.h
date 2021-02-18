#pragma once
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem\UtilityEvents.h"
#include "EventSystem/InputEvents.h"
#include "EventSystem/WindowEvents.h"

class ModuleUI : public IEventListener {
protected:
	//For creating UI elements
	Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory2D;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget2D;
	Microsoft::WRL::ComPtr<IDWriteFactory> m_pTextFactory;
	Microsoft::WRL::ComPtr<ID2D1GeometrySink> m_pSink;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;

	//Window information
	unsigned int m_pWindowWidth;
	unsigned int m_pWindowHeight;

	void UpdateDXHandlers(IEvent& event) noexcept;
public:
	ModuleUI() noexcept;
	virtual ~ModuleUI() = default;
	bool Initialize();

	virtual bool UpdateModules() = 0;
	void UpdateBrush(D2D1::ColorF color, float opacity);

	void BeginFrame();
	void RenderHelpGrid(int gridSize);
	virtual void RenderUI() = 0;
	void EndFrame();
};

