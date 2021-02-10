#include "pch.h"
#include "ModelFactory.h"

ModelFactory ModelFactory::m_me;

ModelFactory& ModelFactory::Get() noexcept
{
	return m_me;
}

void ModelFactory::setDeviceAndContext(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext) {
	this->m_device = device;
	this->m_deviceContext = deviceContext;
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
			OutputDebugString(L"\n\n!!! Error retrieving data from model file !!!\n\n");
#ifdef _DEBUG
			assert(false);
#endif
		}
		else
		{
			std::vector<vertex_tex> vertices;
			std::vector<int> indices;
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
					vtx.normal.x = 1.0f;	// temporary for
					vtx.normal.y = 0.3f;	// vertex color
					vtx.normal.z = 0.1f;	// use section below.

					//if (mesh->HasNormals())
					//{
					//	vtx.normal.x = mesh->mNormals[i].x;
					//	vtx.normal.y = mesh->mNormals[i].y;
					//	vtx.normal.z = mesh->mNormals[i].z;
					//}
					if (mesh->HasTangentsAndBitangents())
					{
						vtx.tangent.x = mesh->mTangents[i].x;
						vtx.tangent.y = mesh->mTangents[i].y;
						vtx.tangent.z = mesh->mTangents[i].z;
						vtx.bitangent.x = mesh->mBitangents[i].x;
						vtx.bitangent.y = mesh->mBitangents[i].y;
						vtx.bitangent.z = mesh->mBitangents[i].z;
					}
					vertices.push_back(vtx);


				}
				for (UINT i = 0u; i < mesh->mNumFaces; ++i)
				{
					aiFace face = mesh->mFaces[i];
					for (UINT j = 0u; j < face.mNumIndices; ++j)
					{
						indices.push_back(face.mIndices[j]);
					}
				}
			}

#ifdef _DEBUG
			loadDebug += std::string("=======================================\n");
			OutputDebugStringA(loadDebug.c_str());
#endif
			model->setVertexBufferSize(static_cast<UINT>(vertices.size()));
			model->setIndexBufferSize(static_cast<UINT>(indices.size()));
			createBuffers(sizeof(vertex_tex), vertices.size(), static_cast<void*>(vertices.data()), indices, model);
			model->Loaded();
		}
	}
#ifdef _DEBUG
	assert(model->NotLoaded() == false);
#endif
	return model;
}

Model* ModelFactory::GeneratePlanet(float x, float y, float z, float r) {
	//Create the sphere vertices and indices. The vertices are just raw float values.
	Model* model = new Model();
	std::vector<float> vertexPositionValues;
	std::vector<int> indices;
	createSphere(r, vertexPositionValues, indices);

	DirectX::XMFLOAT3 center = { x, y, z };
	vertexPositionValues = createHeightOffset(vertexPositionValues.size(), static_cast<void*>(vertexPositionValues.data()), center, r);

	//Convert the data to vertex_col.
	std::vector<vertex_col> vertices;
	for (size_t i = 0; i < vertexPositionValues.size(); i += 4) {
		vertex_col newVertex = {};
		newVertex.position.x = vertexPositionValues[i];
		newVertex.position.y = vertexPositionValues[i + 1];
		newVertex.position.z = vertexPositionValues[i + 2];

		DirectX::XMFLOAT3 distance = {};
		distance.x = newVertex.position.x;
		distance.y = newVertex.position.y;
		distance.z = newVertex.position.z;

		float len = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
		if (len < r/*r - (r / 50)*/ ) {
			newVertex.color.x = 0.0f;
			newVertex.color.y = 0.0f;
			newVertex.color.z = static_cast<float>(std::pow(len / r, 5));
			newVertex.color.w = 1.0f;
		}
		else if (len >= r && len <= r + (r / 15)){
			newVertex.color.x = 0.0f;
			newVertex.color.y = 0.5f;
			newVertex.color.z = 0.0f;
			newVertex.color.w = 1.0f;
		}
		else {
			newVertex.color.x = 1.0f - (0.5f * static_cast<float>(std::pow(((r + (r / 15)) / len), 10)));
			newVertex.color.y = 1.0f - (0.5f * static_cast<float>(std::pow(((r + (r / 15)) / len), 10)));
			newVertex.color.z = 1.0f - (0.5f * static_cast<float>(std::pow(((r + (r / 15)) / len), 10)));
			newVertex.color.w = 1.0f;
		}

		newVertex.bitangent.x = 1.0f;
		newVertex.bitangent.y = 1.0f;
		newVertex.bitangent.z = 1.0f;

		newVertex.tangent.x = 1.0f;
		newVertex.tangent.y = 1.0f;
		newVertex.tangent.z = 1.0f;

		newVertex.normal.x = 1.0f;
		newVertex.normal.y = 1.0f;
		newVertex.normal.z = 1.0f;

		vertices.push_back(newVertex);
	}

	model->setVertexBufferSize(static_cast<UINT>(vertices.size()));
	model->setIndexBufferSize(static_cast<UINT>(indices.size()));

	createBuffers(sizeof(vertex_col), vertices.size(), static_cast<void*>(vertices.data()), indices, model);
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
	unsigned int divisions = static_cast<int>(std::ceil(r / 5));
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
		vertexBuffer.push_back(0.0f); //Trash value for compute shader
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
			triangles.push_back(vertexMap[(reverse) ? v1 : v2]);
			triangles.push_back(vertexMap[(reverse) ? v2 : v1]);
			triangles.push_back(vertexMap[v0]);
		}
	}
}

std::vector<float> ModelFactory::createHeightOffset(size_t size, void* data, DirectX::XMFLOAT3 center, float r)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> srcDataGPUBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> destDataGPUBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> copyToBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> planetConstantsBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srcDataGPUBufferView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> destDataGPUBufferView;

	//SOURCE
	// First we create a buffer in GPU memory
	D3D11_BUFFER_DESC descGPUBuffer;
	ZeroMemory(&descGPUBuffer, sizeof(descGPUBuffer));
	descGPUBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS |
		D3D11_BIND_SHADER_RESOURCE;
	descGPUBuffer.ByteWidth = sizeof(float) * static_cast<UINT>(size);
	descGPUBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descGPUBuffer.StructureByteStride = 16;    // We assume the data is in the
											  // RGB format, 6 bits per chan

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = data;
	HR_X(this->m_device->CreateBuffer(&descGPUBuffer, &InitData, &srcDataGPUBuffer), "CreateBuffer");

	// Now we create a view on the resource. DX11 requires you to send the data
	// to shaders using a "shader view"
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	srcDataGPUBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC descView;
	ZeroMemory(&descView, sizeof(descView));
	descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	descView.BufferEx.FirstElement = 0;

	descView.Format = DXGI_FORMAT_UNKNOWN;
	descView.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

	HR_X(m_device->CreateShaderResourceView(srcDataGPUBuffer.Get(), &descView, &srcDataGPUBufferView), "CreateShaderResourceView");

	//DEST
	//------------------------------------------------------------------------------------------------------------------------

	// The compute shader will need to output to some buffer so here 
	// we create a GPU buffer for that.
	D3D11_BUFFER_DESC descGPUBufferUAV;
	ZeroMemory(&descGPUBufferUAV, sizeof(descGPUBufferUAV));
	descGPUBufferUAV.BindFlags = D3D11_BIND_UNORDERED_ACCESS |
		D3D11_BIND_SHADER_RESOURCE;
	descGPUBufferUAV.ByteWidth = sizeof(float) * static_cast<UINT>(size);
	descGPUBufferUAV.Usage = D3D11_USAGE_DEFAULT;
	descGPUBufferUAV.CPUAccessFlags = 0;
	descGPUBufferUAV.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descGPUBufferUAV.StructureByteStride = 16;    // We assume the output data is 
	// in the RGB format, 6 bits per channel

	HR_X(m_device->CreateBuffer(&descGPUBufferUAV, NULL,
		&destDataGPUBuffer), "CreateBuffer");

	// The view we need for the output is an unordered access view. 
	// This is to allow the compute shader to write anywhere in the buffer.
	D3D11_BUFFER_DESC descBufUAV;
	ZeroMemory(&descBufUAV, sizeof(descBufUAV));
	destDataGPUBuffer->GetDesc(&descBufUAV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC descViewUAV;
	ZeroMemory(&descViewUAV, sizeof(descViewUAV));
	descViewUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descViewUAV.Buffer.FirstElement = 0;

	// Format must be must be DXGI_FORMAT_UNKNOWN, when creating 
	// a View of a Structured Buffer
	descViewUAV.Format = DXGI_FORMAT_UNKNOWN;
	descViewUAV.Buffer.NumElements = descBufUAV.ByteWidth / descBufUAV.StructureByteStride;

	HR_X(m_device->CreateUnorderedAccessView(destDataGPUBuffer.Get(),
		&descViewUAV, &destDataGPUBufferView), "CreateUnorderedAccessView");

	//Constant buffer
	//------------------------------------------------------------------------------------

	D3D11_BUFFER_DESC planetConstantBufferDesc = {};
	planetConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	planetConstantBufferDesc.ByteWidth = sizeof(PlanetConstants);
	planetConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	planetConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	planetConstantBufferDesc.MiscFlags = 0;
	planetConstantBufferDesc.StructureByteStride = 0;
	HR_X(this->m_device->CreateBuffer(&planetConstantBufferDesc,
		nullptr,
		&planetConstantsBuffer),
		"CreateBuffer");

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	PlanetConstants* constantBufferData;

	m_deviceContext->Map(
		planetConstantsBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource
	);

	constantBufferData = (PlanetConstants*)mappedSubresource.pData;

	constantBufferData->center = center;
	constantBufferData->radius = r;

	m_deviceContext->Unmap(planetConstantsBuffer.Get(), 0);

	//Set everything
	//------------------------------------------------------------------------------------

	m_deviceContext->CSSetConstantBuffers(0, 1, planetConstantsBuffer.GetAddressOf());
	m_deviceContext->CSSetShaderResources(0, 1, srcDataGPUBufferView.GetAddressOf());
	m_deviceContext->CSSetUnorderedAccessViews(0, 1, destDataGPUBufferView.GetAddressOf(), NULL);
	m_deviceContext->Dispatch(static_cast<UINT>(size) / 8, 2u, 1u);

	ID3D11ShaderResourceView* nullSRV[] = { NULL };
	m_deviceContext->CSSetShaderResources(0, 1, nullSRV);
	ID3D11UnorderedAccessView* nullUAV[] = { NULL };
	m_deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

	//Copy from dest to readable buffer.
	//------------------------------------------------------------------------------------

	//Create readable buffer
	D3D11_BUFFER_DESC copyToBufferDesc;
	ZeroMemory(&copyToBufferDesc, sizeof(copyToBufferDesc));
	copyToBufferDesc.BindFlags = 0;
	copyToBufferDesc.ByteWidth = sizeof(float) * static_cast<UINT>(size);
	copyToBufferDesc.Usage = D3D11_USAGE_STAGING;
	copyToBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	copyToBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	copyToBufferDesc.StructureByteStride = 16;    // We assume the output data is 
	// in the RGB format, 6 bits per channel

	HR_X(m_device->CreateBuffer(&copyToBufferDesc, NULL,
		&copyToBuffer), "CreateBuffer");

	m_deviceContext->CopyResource(copyToBuffer.Get(), destDataGPUBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE doneVertices;
	m_deviceContext->Map(copyToBuffer.Get(), 0, D3D11_MAP_READ, 0, &doneVertices);

	float* doneData = (float*)doneVertices.pData;
	std::vector<float> donePositionValues;
	for (size_t i = 0; i < size; i++) {
		donePositionValues.push_back(*doneData);
		doneData++;
	}

	m_deviceContext->Unmap(copyToBuffer.Get(), 0);

	return donePositionValues;
}

void ModelFactory::createBuffers(UINT stride, size_t size, void* data, const std::vector<int>& indices, Model* model) {
	model->setStride(stride);
	model->setOffset(0u);

	//vertexbuffer
	D3D11_BUFFER_DESC vertexBufferDescriptor = {};
	vertexBufferDescriptor.ByteWidth = sizeof(vertex_col) * static_cast<UINT>(size);
	vertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA sr_data = { 0 };
	sr_data.pSysMem = data;

	HR_X(this->m_device->CreateBuffer(&vertexBufferDescriptor,
									  &sr_data,
									  &model->getVertexBuffer()), 
									  "CreateBuffer");
	//Indexbuffer
	D3D11_BUFFER_DESC indexBufferDescriptor = {};
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * static_cast<UINT>(indices.size());
	indexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = indices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	HR_X(this->m_device->CreateBuffer(&indexBufferDescriptor,
									  &InitData,
									  &model->getIndexBuffer()), 
									  "CreateBuffer");
	//Matrixbuffer for shader
	D3D11_BUFFER_DESC matrixBufferDesc = {};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	HR_X(this->m_device->CreateBuffer(&matrixBufferDesc, 
									  nullptr, 
									  &model->getMatrixBuffer()), 
									  "CreateBuffer");
}


void ModelFactory::PreparePlanetDisplacement() {
	BindIDEvent me(BindID::ID_PlanetHeight);
	EventBuss::Get().Delegate(me);
}

Model* ModelFactory::GenerateSun(float x, float y, float z, float r) {
	//Create the sphere vertices and indices. The vertices are just raw float values.
	Model* model = new Model();
	std::vector<float> vertexPositionValues;
	std::vector<int> indices;
	createSphere(r, vertexPositionValues, indices);

	std::vector<vertex_col> vertices;
	for (unsigned int i = 0; i < vertexPositionValues.size(); i += 4) {
		vertex_col newVertex;
		newVertex.position.x = vertexPositionValues[i];
		newVertex.position.y = vertexPositionValues[i + 1];
		newVertex.position.z = vertexPositionValues[i + 2];

		newVertex.color.x = 255.0f / 255.0f;
		newVertex.color.y = 165.0f / 255.0f;
		newVertex.color.z = 0.0f;
		newVertex.color.w = 1.0f;

		newVertex.bitangent.x = 1.0f;
		newVertex.bitangent.y = 1.0f;
		newVertex.bitangent.z = 1.0f;

		newVertex.tangent.x = 1.0f;
		newVertex.tangent.y = 1.0f;
		newVertex.tangent.z = 1.0f;

		newVertex.normal.x = 1.0f;
		newVertex.normal.y = 1.0f;
		newVertex.normal.z = 1.0f;

		vertices.push_back(newVertex);
	}

	model->setVertexBufferSize(static_cast<UINT>(vertices.size()));
	model->setIndexBufferSize(static_cast<UINT>(indices.size()));

	createBuffers(sizeof(vertex_col), vertices.size(), static_cast<void*>(vertices.data()), indices, model);
	return model;
}