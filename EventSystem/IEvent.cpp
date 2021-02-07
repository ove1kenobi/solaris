#include "..\pch.h"
#include "IEvent.h"

IEvent::IEvent() noexcept
	: m_Handled{ false }
{

}

const bool& IEvent::IsHandled() const noexcept
{
	return m_Handled;
}

void IEvent::SetAsHandled() noexcept
{
	m_Handled = true;
}
