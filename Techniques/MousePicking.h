#pragma once
#include "..\EventSystem\IEventListener.h"
#include "..\EventSystem\EventPublisher.h"
#include "..\EventSystem\InputEvents.h"
#include "..\EventSystem\UtilityEvents.h"
#include "..\EventSystem\WindowEvents.h"
#include "..\EventSystem\UIEvents.h"
#include "..\PlayerCamera.h"
#include "..\GameObject.h"
#include "../Planet.h"

class MousePicking : public IEventListener, public EventPublisher
{
private:
	Camera* m_pCamera;
	unsigned int m_ClientWidth;
	unsigned int m_ClientHeight;
	GameObject* m_pPickedObject;
	float m_DistanceToObject;
private:
	void OnEvent(IEvent& event) noexcept override;
public:
	MousePicking() noexcept;
	virtual ~MousePicking() noexcept = default;
	[[nodiscard]] const bool Initialize() const noexcept;
	void DoIntersectionTests(const unsigned int& x, const unsigned int& y, const std::vector<GameObject*>& gameObjects) noexcept;
	void DisplayPickedObject() noexcept;
};