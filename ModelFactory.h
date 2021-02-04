#pragma once
#include "DXDebug.h"
#include <unordered_map>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Model.h"
#include "ourMath.h"

class ModelFactory
{
private:
	Assimp::Importer m_importer;
	static ModelFactory m_me;
	std::unordered_map<std::string, Model> m_loadedModels;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
private:
	ModelFactory() noexcept = default;
	~ModelFactory() noexcept = default;
	ModelFactory& operator=(const ModelFactory& other) = delete;

	//Called in GenerateSphere to create the sphere.
	void createSphere(float r, std::vector<float> &vertexBuffer, std::vector<int> &indexBuffer);
	//Called in createSphere to create a new triangle.
	void createTriangleFace(
		std::vector<int> edge1,
		std::vector<int> edge2,
		std::vector<int> edge3,
		bool reverse,
		std::vector<DirectX::XMFLOAT3>& vertices,
		std::vector<int>& triangles,
		unsigned int divisions
	);

	void createBuffers(UINT stride, size_t size, void* data, const std::vector<int>& indexBuffer, Model* model);
public:
	static ModelFactory& Get() noexcept;
	Model* GetModel(std::string filePath);
	Model* GenerateSphere(float x, float y, float z, float r);
	void setDevice(Microsoft::WRL::ComPtr<ID3D11Device> device);

	struct MatrixBuffer {
		DirectX::XMMATRIX WMatrix;
		DirectX::XMMATRIX VMatrix;
		DirectX::XMMATRIX PMatrix;
	};
};
