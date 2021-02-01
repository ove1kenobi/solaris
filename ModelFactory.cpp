#include "ModelFactory.h"

ModelFactory ModelFactory::m_me;

ModelFactory* ModelFactory::Get() noexcept
{
	return &m_me;
}

Model* ModelFactory::GetModel(std::string filePath)
{
	Model model = m_loadedModels[filePath];
	if (model.NotLoaded())
	{
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
#ifdef _DEBUG
			std::string loadDebug = std::string("=======================================\nLoading model: ") + filePath + std::string("\n");
			loadDebug += std::string("Meshes: ") + std::to_string(scene->mNumMeshes) + std::string("\t");
			loadDebug += std::string("Materials: ") + std::to_string(scene->mNumMaterials) + std::string("\t");
			loadDebug += std::string("Textures: ") + std::to_string(scene->mNumMaterials) + std::string("\n");
#endif
			for (UINT iMesh = 0u; iMesh < scene->mNumMeshes; ++iMesh)
			{
				const aiMesh* mesh = scene->mMeshes[iMesh];
#ifdef _DEBUG
				loadDebug += std::string("Mesh: #") + std::to_string(iMesh) + std::string("\tVertices: ") + std::to_string(mesh->mNumVertices) + std::string("\n");
#endif	
				for (UINT i = 0u; i < mesh->mNumVertices; ++i)
				{
					vertex_tex vtx = {};
					vtx.position.x = mesh->mVertices[i].x;
					vtx.position.y = mesh->mVertices[i].y;
					vtx.position.z = mesh->mVertices[i].z;
					if (mesh->HasTextureCoords(iMesh))
					{
						vtx.texcoord.x = mesh->mTextureCoords[iMesh][i].x;
						vtx.texcoord.y = mesh->mTextureCoords[iMesh][i].y;
					}
					if (mesh->HasNormals())
					{
						vtx.normal.x = mesh->mNormals[i].x;
						vtx.normal.y = mesh->mNormals[i].y;
						vtx.normal.z = mesh->mNormals[i].z;
					}
					if (mesh->HasTangentsAndBitangents())
					{
						vtx.tangent.x = mesh->mTangents[i].x;
						vtx.tangent.y = mesh->mTangents[i].y;
						vtx.tangent.z = mesh->mTangents[i].z;
						vtx.bitangent.x = mesh->mBitangents[i].x;
						vtx.bitangent.y = mesh->mBitangents[i].y;
						vtx.bitangent.z = mesh->mBitangents[i].z;
					}
					model.AddVertex(vtx);

					for (UINT i = 0u; i < mesh->mNumFaces; ++i)
					{
						aiFace face = mesh->mFaces[i];
						for (UINT j = 0u; j < face.mNumIndices; ++j)
						{
							model.AddIndex(face.mIndices[j]);
						}
					}
				}
			}
#ifdef _DEBUG
			loadDebug += std::string("=======================================\n");
			OutputDebugStringA(loadDebug.c_str());
#endif
		}
	}
	return nullptr;
}
