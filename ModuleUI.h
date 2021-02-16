#pragma once
#include "EventSystem/IEventListener.h"
#include "EventSystem\UtilityEvents.h"

class ModuleUI : IEventListener {
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

	[[nodiscard]] const bool Initialize() noexcept;

	bool UpdateBrush();
	void BeginFrame();
	void RenderHelpGrid(int gridSize);
	virtual void RenderUI() = 0;
	void EndFrame();

	void OnEvent(IEvent& event) noexcept;
};

/*
We have car, truck, bike, motorbike
They are all type veichle
Then we have a road, where a known amount of vehicles can be on.
Road decides what vehicles is allowed to be on it when.
*/

