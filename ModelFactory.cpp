#include "ModelFactory.h"

ModelFactory* ModelFactory::m_me = nullptr;

ModelFactory::ModelFactory()
{

}

ModelFactory::~ModelFactory()
{

}

ModelFactory* ModelFactory::GetInstance()
{
	if (m_me == nullptr)
	{
		m_me = new ModelFactory();
	}
	return m_me;
}

Model* ModelFactory::GetModel(std::string& filePath)
{
	Model model = m_loadedModels[filePath];
	if (model.NotLoaded())
	{
		const aiScene* pScene = m_importer.ReadFile(filePath.c_str(),
			  aiProcess_Triangulate
			| aiProcess_ConvertToLeftHanded
			| aiProcess_CalcTangentSpace
			| aiProcess_GenNormals
			| aiProcess_JoinIdenticalVertices
		);

		if (pScene != nullptr)
		{
			OutputDebugString(L"Error retrieving data from model file");
		}
		else
		{

		}
	}
	return &model;
}
