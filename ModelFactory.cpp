#include "ModelFactory.h"

ModelFactory ModelFactory::m_me;

ModelFactory& ModelFactory::Get() noexcept
{
	return m_me;
}

Model* ModelFactory::GetModel(std::string filePath)
{
	Model* model = &m_loadedModels[filePath];
	if (model->NotLoaded())
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
					model->AddVertex(vtx);

					for (UINT i = 0u; i < mesh->mNumFaces; ++i)
					{
						aiFace face = mesh->mFaces[i];
						for (UINT j = 0u; j < face.mNumIndices; ++j)
						{
							model->AddIndex(face.mIndices[j]);
						}
					}
				}
			}

#ifdef _DEBUG
			loadDebug += std::string("=======================================\n");
			OutputDebugStringA(loadDebug.c_str());
#endif
		}
		model->Loaded();
	}
	return model;
}

Model ModelFactory::GenerateSphere(float x, float y, float z, float r) {
	
	Model model;
	std::vector<float> vertexBuffer;
	std::vector<int> indexBuffer;
	createSphere(r, vertexBuffer, indexBuffer);

	for (unsigned int i = 0; i < vertexBuffer.size(); i += 3) {
		vertex_col newVertex;
		newVertex.position.x = vertexBuffer[i];
		newVertex.position.y = vertexBuffer[i + 1];
		newVertex.position.z = vertexBuffer[i + 2];

		newVertex.color.x = 0.5;
		newVertex.color.y = 0.5;
		newVertex.color.z = 0.5;

		newVertex.bitangent.x = 0;
		newVertex.bitangent.y = 0;
		newVertex.bitangent.z = 0;

		newVertex.tangent.x = 0;
		newVertex.tangent.y = 0;
		newVertex.tangent.z = 0;

		newVertex.normal.x = 0;
		newVertex.normal.y = 0;
		newVertex.normal.z = 0;

		model.AddVertex(newVertex);
	}

	for (unsigned int i = 0; i < indexBuffer.size(); i++) {
		model.AddIndex(indexBuffer[i]);
	}

	return model;
}

void ModelFactory::createSphere(float r, std::vector<float> &vertexBuffer, std::vector<int> &indexBuffer) {
	//Starting octahedron points.
	DirectX::XMFLOAT3 p0, p1, p2, p3, p4, p5;
	//So that the triangles have the same length on all 3 sides.
	float x = sqrt((r * r) / 2);
	float y = r;
	float z = sqrt((r * r) / 2);
	//Points of the octahedron
	p0 = {  0,  y,  0 }; //Top
	p1 = { -x,  0, -z }; //Front left
	p2 = { -x,  0,  z }; //Back left
	p3 = {  x,  0,  z }; //Back right
	p4 = {  x,  0, -z }; //Front right
	p5 = {  0, -y,  0 }; //Bottom

	std::vector<DirectX::XMFLOAT3> octVertices = { p0, p1, p2, p3, p4, p5 };

	//vector representing the lines of the octahedron.
	std::vector<std::vector<int>> lines = {
		{ 0, 1 }, //0	//Top to the 4 different "middle" corners
		{ 0, 2 }, //1
		{ 0, 3 }, //2
		{ 0, 4 }, //3
		{ 1, 2 }, //4	//Around the "belt" of the octahedron
		{ 2, 3 }, //5
		{ 3, 4 }, //6
		{ 4, 1 }, //7
		{ 5, 1 }, //8	//Bottom to the 4 different "middle" corners
		{ 5, 2 }, //9
		{ 5, 3 }, //10
		{ 5, 4 }  //11
	};

	//Represt the triangles with the lines. (Indices of the last vector)
	std::vector<std::vector<int>> baseTriangles = {
		//Top Triangles
		{ 0, 4, 1 },
		{ 1, 5, 2 },
		{ 2, 6, 3 },
		{ 3, 7, 0 },
		//Bottom Triangles
		{ 8, 9, 4 },
		{ 9, 10, 5 },
		{ 10, 11, 6 },
		{ 11, 8, 7 }
	};

	//Calculate the number of divisions that are to be made of each edge. 100 easily changable.
	unsigned int divisions = 0/*static_cast<int>(std::ceil(r / 10))*/;
	//Number of vertices on 1 face.
	unsigned int vertsPerTriangle = ((divisions + 3) * (divisions + 3) - (divisions + 3)) / 2;
	//Number of triangles on 1 face.
	unsigned int trianglesPerTriangle = (divisions + 1) * (divisions + 1);
	//Total vertices on the whole octahedron.
	unsigned int vertsTotal = (vertsPerTriangle * 8) - ((divisions + 2) * 12) + 6;

	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<int> triangles;

	for (int i = 0; i < 6; i++)
		vertices.push_back(octVertices[i]);

	//Create the 12 edges, with "divisions" represents the mount of vertices added along the line.
	std::vector<std::vector<int>> completeEdges;
	for (int i = 0; i < lines.size(); i++) {
		DirectX::XMFLOAT3 startVert = vertices[lines[i][0]];
		DirectX::XMFLOAT3 endVert = vertices[lines[i][1]];

		std::vector<int> edgeVertexIndices;
		//Add the first vertex
		edgeVertexIndices.push_back(lines[i][0]);

		//Add new vertices along this edge.
		for (unsigned int j = 0; j < divisions; j++) {
			float t = (j + 1.0f) / (divisions + 1.0f);
			edgeVertexIndices.push_back(static_cast<int>(vertices.size()));
			vertices.push_back(slerp(startVert, endVert, t));
		}

		//Add the last vertex
		edgeVertexIndices.push_back(lines[i][1]);
		completeEdges.push_back(edgeVertexIndices);
	}

	//Create the new faces.
	for (int i = 0; i < baseTriangles.size(); i++) {
		bool reverse = i >= 4;
		createTriangleFace(completeEdges[baseTriangles[i][0]], completeEdges[baseTriangles[i][1]], completeEdges[baseTriangles[i][2]], reverse, vertices, triangles, divisions);
	}

	for (int i = 0; i < vertices.size(); i++) {
		vertexBuffer.push_back(vertices[i].x);
		vertexBuffer.push_back(vertices[i].y);
		vertexBuffer.push_back(vertices[i].z);
	}

	indexBuffer = triangles;
}

void ModelFactory::createTriangleFace(
	std::vector<int> edge1,
	std::vector<int> edge2,
	std::vector<int> edge3,
	bool reverse,
	std::vector<DirectX::XMFLOAT3>& vertices,
	std::vector<int>& triangles,
	unsigned int divisions
) {
	int pointsOnEdge = static_cast<int>(edge1.size());
	std::vector<int> vertexMap;
	vertexMap.push_back(edge1[0]);

	//To not get the 2 points we already have.
	for (int i = 1; i < pointsOnEdge - 1; i++) {
		//First side's vertex
		vertexMap.push_back(edge1[i]);

		//Middle vertices
		DirectX::XMFLOAT3 edge1Vertex;
		DirectX::XMFLOAT3 edge2Vertex;

		if (!reverse) {
			edge1Vertex = vertices[edge1[i]];
			edge2Vertex = vertices[edge3[i]];
		}
		else {
			edge1Vertex = vertices[edge1[i]];
			edge2Vertex = vertices[edge2[i]];
		}
		int innerPoints = i - 1;
		for (int j = 0; j < innerPoints; j++) {
			float t = (j + 1.0f) / (innerPoints + 1.0f);
			vertexMap.push_back(static_cast<int>(vertices.size()));
			vertices.push_back(slerp(edge1Vertex, edge2Vertex, t));
		}
		//2nd side's vertex
		if (!reverse) {
			vertexMap.push_back(edge3[i]);
		}
		else {
			vertexMap.push_back(edge2[i]);
		}

	}

	if (!reverse) {
		//Add bottom edge vertices
		for (int i = 0; i < pointsOnEdge; i++)
			vertexMap.push_back(edge2[i]);
	}
	else {
		//Add bottom edge vertices
		for (int i = 0; i < pointsOnEdge; i++)
			vertexMap.push_back(edge3[i]);
	}


	//Triangulate
	//Same as divisions at the start of the createSphere function. + 1
	int rows = divisions + 1;/*static_cast<int>(std::ceil(this->m_radius / 100)) + 1;*/
	for (int row = 0; row < rows; row++) {
		//Vertices that are on down-left edge follow quadratic sequence.
		//Calculate by: (n^2 - n) / 2
		int topVertex = ((row + 1) * (row + 1) - row - 1) / 2;
		int bottomVertex = ((row + 2) * (row + 2) - row - 2) / 2;

		int trianglesInRow = (row * 2) + 1;
		for (int column = 0; column < trianglesInRow; column++) {
			int v0;
			int v1;
			int v2;

			if (column % 2 == 0) {
				v0 = topVertex;
				v1 = bottomVertex + 1;
				v2 = bottomVertex;
				topVertex++;
				bottomVertex++;
			}
			else {
				v0 = topVertex;
				v1 = bottomVertex;
				v2 = topVertex - 1;
			}

			triangles.push_back(vertexMap[v0]);
			triangles.push_back(vertexMap[(reverse) ? v2 : v1]);
			triangles.push_back(vertexMap[(reverse) ? v1 : v2]);
		}
	}
}
