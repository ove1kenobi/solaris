#include "..\pch.h"
#include "InputLayout.h"

void InputLayout::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext && m_pInputLayout);
#endif
	pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
}

void InputLayout::Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext);
#endif
	pDeviceContext->IASetInputLayout(nullptr);
}

const bool InputLayout::Create(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, const VertexShader& vertexShader, unsigned int type)
{
	/*The input element descriptor will be continously expanded.
	  The descriptor should probably be created outside of this function. */
	if (type != LAYOUT_COSMIC && type != LAYOUT_PLAYER && type != LAYOUT_MINIMAL)
	{
		return false;
	}
	if (type == LAYOUT_COSMIC)
	{
		D3D11_INPUT_ELEMENT_DESC ieDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		HR(pDevice->CreateInputLayout(ieDesc,
									  ARRAYSIZE(ieDesc),
									  vertexShader.GetVertexShaderBlob()->GetBufferPointer(),
									  vertexShader.GetVertexShaderBlob()->GetBufferSize(),
									  &m_pInputLayout),
									  "CreateInputLayout");
	}
	else if (type == LAYOUT_PLAYER)
	{
		D3D11_INPUT_ELEMENT_DESC ieDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		HR(pDevice->CreateInputLayout(ieDesc,
									  ARRAYSIZE(ieDesc),
									  vertexShader.GetVertexShaderBlob()->GetBufferPointer(),
									  vertexShader.GetVertexShaderBlob()->GetBufferSize(),
									  &m_pInputLayout),
									  "CreateInputLayout");
	}
	else if (type == LAYOUT_MINIMAL)
	{
		D3D11_INPUT_ELEMENT_DESC ieDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		HR(pDevice->CreateInputLayout(ieDesc,
									  ARRAYSIZE(ieDesc),
									  vertexShader.GetVertexShaderBlob()->GetBufferPointer(),
									  vertexShader.GetVertexShaderBlob()->GetBufferSize(),
									  &m_pInputLayout),
									  "CreateInputLayout");
	}
	return true;
}