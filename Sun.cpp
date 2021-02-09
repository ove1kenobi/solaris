#include "pch.h"
#include "Sun.h"
bool Sun::Initialize() 
{
	//The cosmic body
	this->init(0.0f, 0.0f, 0.0f, 900.0f, 0.0f, 0.0f, 1);
	//The light
	this->light.Init({ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 100.0f, 200.0f, 300.0f },
		2000.0f);
	return true;
}
