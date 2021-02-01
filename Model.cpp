#include "Model.h"

Model::Model() : m_notLoaded{ true }
{

}

bool Model::NotLoaded()
{
	return m_notLoaded;
}
