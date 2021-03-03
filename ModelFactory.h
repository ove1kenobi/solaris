#pragma once
#include <atlbase.h>
#include "DXDebug.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Model.h"
#include "ourMath.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include <algorithm>
class ModelFactory : public EventPublisher
{
private:
	Assimp::Importer m_importer;
	static ModelFactory m_me;
	std::unordered_map<std::string, Model> m_loadedModels;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;

	std::mutex m_mutex;
private:
	ModelFactory() noexcept = default;
	~ModelFactory() noexcept = default;
	ModelFactory& operator=(const ModelFactory& other) = delete;

	//Called in GenerateSphere to create the sphere.
	void createSphere(float r, UINT setDivisions, std::vector<float> &vertexBuffer, std::vector<UINT> &indexBuffer);
	//Called in createSphere to create a new triangle.
	void createTriangleFace(
		std::vector<int> edge1,
		std::vector<int> edge2,
		std::vector<int> edge3,
		bool reverse,
		std::vector<DirectX::XMFLOAT3>& vertices,
		std::vector<UINT>& triangles,
		unsigned int divisions
	);
	std::vector<float> createHeightOffset(size_t size, void* data, DirectX::XMFLOAT3 center, float r);
	std::vector<DirectX::XMFLOAT3> calcNormals(std::vector<float> vertices, std::vector<UINT> indices);

	void createBuffers(UINT stride, size_t size, void* data, const std::vector<UINT>& indexBuffer, Model* model);

	void setColorVertex(float r, UINT type, float elevation, float poleAngle, float vertexAngle, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT4* color);
public:
	static ModelFactory& Get() noexcept;
	Model* GetModel(std::string filePath);
	Model* GeneratePlanet(float x, float y, float z, float r, UINT type, DirectX::XMFLOAT3 yAxis);
	Model* GenerateWaterSphere(float x, float y, float z, float r);
	Model* GenerateSun(float x, float y, float z, float r);
	Model* GenerateOrbit(float major_semi_axis, float minor_semi_axis);
	void PreparePlanetDisplacement();
	void setDeviceAndContext(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);
	void CreateMatrixBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& matrixBuf);

	struct MatrixBuffer {
		DirectX::XMMATRIX WMatrix;
		DirectX::XMMATRIX WVPMatrix;
	};

	struct PlanetConstants {
		DirectX::XMFLOAT3 center;
		float radius;
	};

	struct RadiusBuffer {
		float radius;
		DirectX::XMFLOAT3 padding;
	};

	struct WorldPosition {
		float x;
		float y;
		float z;
		float w; //Trash value
	};
};
