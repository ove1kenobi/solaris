#include "CosmicBody.h"

CosmicBody::CosmicBody() noexcept
	:	m_radius{ 0.0f }, m_yAxis{ 0.0f, 1.0f, 0.0f, 0.0f }
{
	
}

bool CosmicBody::init(float x, float y, float z, float r, float xRot, float zRot) {
	this->m_radius = r;
	this->m_center.x = x;
	this->m_center.y = y;
	this->m_center.z = z;
	this->m_pitch = xRot;
	this->m_roll = zRot;

	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(this->m_pitch);
	DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(this->m_roll);
	DirectX::XMVECTOR resultY = DirectX::XMLoadFloat4(&this->m_yAxis);
	resultY = DirectX::XMVector4Transform(resultY, rotX);
	resultY = DirectX::XMVector4Transform(resultY, rotZ);
	DirectX::XMStoreFloat4(&this->m_yAxis, resultY);

	DirectX::XMStoreFloat4x4(&this->m_wMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(x, y, z);
	DirectX::XMMATRIX final = trans;

	DirectX::XMStoreFloat4x4(&this->m_wMatrix, final);
	

	this->m_model = ModelFactory::Get().GenerateSphere(x, y, z, r);
	
	//createSphere();

	return true;
}

bool CosmicBody::update(DirectX::XMMATRIX VMatrix, DirectX::XMMATRIX PMatrix, const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {
	DirectX::XMFLOAT4 yAxis;
	
	yAxis.x = this->m_wMatrix._12;
	yAxis.y = this->m_wMatrix._22;
	yAxis.z = this->m_wMatrix._32;
	
	//DirectX::XMMATRIX wMatrix = DirectX::XMLoadFloat4x4(&this->m_wMatrix);
	/*
	DirectX::XMFLOAT4X4 zero;
	DirectX::XMStoreFloat4x4(&zero, DirectX::XMMatrixIdentity());
	
	zero._41 = 0;
	zero._42 = 0;
	zero._43 = 0;
	zero._44 = 0;
	DirectX::XMMATRIX zeroMatrix = DirectX::XMLoadFloat4x4(&zero);
	wMatrix = DirectX::XMMatrixMultiply(zeroMatrix, wMatrix);
	*/
	
	static float angle = 0.0f;
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat4(&yAxis), (1000 / this->m_radius) * angle);
	
	DirectX::XMMATRIX transMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 transMatrixFloats;
	DirectX::XMStoreFloat4x4(&transMatrixFloats, transMatrix);
	transMatrixFloats._41 = getTransVector().x;
	transMatrixFloats._42 = getTransVector().y;
	transMatrixFloats._43 = getTransVector().z;
	transMatrix = DirectX::XMLoadFloat4x4(&transMatrixFloats);

	//wMatrix = DirectX::XMMatrixTranslation(this->m_wMatrix._41, this->m_wMatrix._42, this->m_wMatrix._43);
	DirectX::XMMATRIX result = scaleMatrix * rotMatrix * transMatrix;
	DirectX::XMStoreFloat4x4(&this->m_wMatrix, result);
	angle += 0.0005 * this->m_timer.DeltaTime();

	//Update the matrixBuffer.
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ModelFactory::MatrixBuffer* data;

	DirectX::XMMATRIX WMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&this->m_wMatrix));
	VMatrix = DirectX::XMMatrixTranspose(VMatrix);
	PMatrix = DirectX::XMMatrixTranspose(PMatrix);

	deviceContext->Map(
		this->m_model->getMatrixBuffer().Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedSubresource
	);

	data = (ModelFactory::MatrixBuffer*)mappedSubresource.pData;

	data->WMatrix = WMatrix;
	data->VMatrix = VMatrix;
	data->PMatrix = PMatrix;

	deviceContext->Unmap(this->m_model->getMatrixBuffer().Get(), 0);
	return true;
}
/*
void CosmicBody::createSphere() {
	//Starting octahedron points.
	DirectX::XMFLOAT3 p0, p1, p2, p3, p4, p5;
	//So that the triangles have the same length on all 3 sides.
	float x = sqrt((this->m_radius * this->m_radius) / 2);
	float y = this->m_radius;
	float z = sqrt((this->m_radius * this->m_radius) / 2);
	//Points of the octahedron
	p0 = {  0,  y	,  0 };	//Top
	p1 = { -x,  0				, -z };	//Front left
	p2 = { -x,  0				,  z }; //Back left
	p3 = {  x,  0				,  z }; //Back right
	p4 = {  x,  0				, -z }; //Front right
	p5 = {  0, -y	,  0 }; //Bottom

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
	unsigned int divisions = this->m_divisions;/*static_cast<int>(std::ceil(this->m_radius / 100));
	//Number of vertices on 1 face.
	unsigned int vertsPerTriangle = ((divisions + 3) * (divisions + 3) - (divisions + 3)) / 2;
	//Number of triangles on 1 face.
	unsigned int trianglesPerTriangle = (divisions + 1) * (divisions + 1);
	//Total vertices on the whole octahedron.
	unsigned int vertsTotal = (vertsPerTriangle * 8) - ((divisions + 2) * 12) + 6;

	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<int> triangles;

	for(int i = 0; i < 6; i++)
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
		createTriangleFace(completeEdges[baseTriangles[i][0]], completeEdges[baseTriangles[i][1]], completeEdges[baseTriangles[i][2]], reverse, vertices, triangles);
	}

	for (int i = 0; i < vertices.size(); i++) {
		this->m_vertexBuffer.push_back(vertices[i].x);
		this->m_vertexBuffer.push_back(vertices[i].y);
		this->m_vertexBuffer.push_back(vertices[i].z);
	}
	
	this->m_indexBuffer = triangles;
}

void CosmicBody::createTriangleFace(std::vector<int> edge1, std::vector<int> edge2, std::vector<int> edge3, bool reverse, std::vector<DirectX::XMFLOAT3> &vertices, std::vector<int> &triangles) {
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
	int rows = this->m_divisions + 1;/*static_cast<int>(std::ceil(this->m_radius / 100)) + 1;
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
*/

void CosmicBody::bindUniques(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {

	deviceContext->IASetVertexBuffers(
		0u,
		1u,
		this->m_model->getVertexBuffer().GetAddressOf(),
		&this->m_model->getStride(),
		&this->m_model->getOffset()
	);

	// Set the buffer.
	deviceContext->IASetIndexBuffer(this->m_model->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->VSSetConstantBuffers(0, 1, this->m_model->getMatrixBuffer().GetAddressOf());
}