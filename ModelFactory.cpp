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

Model* ModelFactory::GetModel(std::string filePath)
{
	Model model = m_loadedModels[filePath];
	if (model.NotLoaded())
	{
		OutputDebugString(L"Retrieving model data...");
		const aiScene* scene = m_importer.ReadFile(filePath.c_str(),
			  aiProcess_Triangulate
			| aiProcess_ConvertToLeftHanded
			| aiProcess_CalcTangentSpace
			| aiProcess_GenNormals
			| aiProcess_JoinIdenticalVertices
		);

		if (scene == nullptr)
		{
			OutputDebugString(L"Error retrieving data from model file");
		}
		else
		{
			std::string loadDebug = std::string("Loading model: ") + filePath + std::string("\n");
			loadDebug += std::string("Meshes: ") + std::to_string(scene->mNumMeshes) + std::string("\t");
			loadDebug += std::string("Materials: ") + std::to_string(scene->mNumMaterials) + std::string("\t");
			loadDebug += std::string("Textures: ") + std::to_string(scene->mNumMaterials) + std::string("\n");
			OutputDebugStringA(loadDebug.c_str());

			for (UINT iMesh = 0u; iMesh < scene->mNumMeshes; ++iMesh)
			{
				const aiMesh* mesh = scene->mMeshes[iMesh];
				loadDebug = std::string("Mesh: #") + std::to_string(iMesh) + std::string("\tVertices: ") + std::to_string(mesh->mNumVertices) + std::string("\n");
				OutputDebugStringA(loadDebug.c_str());
			}

			delete scene;
		}
	}
	return nullptr;
}
