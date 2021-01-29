#pragma once
#include "assimp/Importer.hpp"

class ModelFactory
{
private:
	ModelFactory* m_me;
private:
	ModelFactory();
	ModelFactory& operator=(const ModelFactory& other) = delete;
};
