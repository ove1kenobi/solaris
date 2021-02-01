#pragma once
#include <string>
#include <unordered_map>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "Model.h"

class ModelFactory
{
private:
	Assimp::Importer m_importer;
	static ModelFactory* m_me;
	std::unordered_map<std::string, Model> m_loadedModels;
private:
	ModelFactory();
	~ModelFactory();
	ModelFactory& operator=(const ModelFactory& other) = delete;
public:
	static ModelFactory* GetInstance();
};
