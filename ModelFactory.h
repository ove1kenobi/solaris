#pragma once
#include <string>
#include <unordered_map>
#include <windows.h>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Model.h"

class ModelFactory
{
private:
	Assimp::Importer m_importer;
	static ModelFactory m_me;
	std::unordered_map<std::string, Model> m_loadedModels;
private:
	ModelFactory() noexcept = default;
	~ModelFactory() noexcept = default;
	ModelFactory& operator=(const ModelFactory& other) = delete;
public:
	static ModelFactory& Get() noexcept;
	Model* GetModel(std::string filePath);
};
