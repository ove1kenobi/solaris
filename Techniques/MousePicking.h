#pragma once
#include "..\EventSystem\IEventListener.h"
#include "..\EventSystem\EventPublisher.h"
#include "..\EventSystem\InputEvents.h"
#include "..\EventSystem\UtilityEvents.h"
#include "..\PlayerCamera.h"
class MousePicking : public IEventListener, public EventPublisher
{
private:
	Camera* m_pCamera;
	unsigned int m_ClientWidth;
	unsigned int m_ClientHeight;
private:
	void OnEvent(IEvent& event) noexcept override;
	void MakeRay(const unsigned int& x, const unsigned int& y) noexcept;
public:
	MousePicking() noexcept;
	virtual ~MousePicking() = default;
	[[nodiscard]] const bool Initialize() const noexcept;
};
