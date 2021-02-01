#include "IBindable.h"

IBindable::IBindable()
	: m_IsBound{ false }
{

}

const bool& IBindable::IsBound() const noexcept
{
	return m_IsBound;
}