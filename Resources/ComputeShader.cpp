#include "..\pch.h"
#include "ComputeShader.h"

void ComputeShader::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext && m_pComputeShader);
#endif
	pDeviceContext->CSSetShader(m_pComputeShader.Get(), nullptr, 0u);
}

void ComputeShader::Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext);
#endif
	pDeviceContext->CSSetShader(nullptr, nullptr, 0u);
}

const bool ComputeShader::Create(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, const LPCWSTR& fileName)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{ nullptr };
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	HR_E(D3DCompileFromFile(fileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"cs_main",
		"cs_5_0",
		flags,
		0u,
		&m_pShaderBlob,
		&pErrorBlob),
		pErrorBlob,
		"D3DCompileFromFile");

	HR(pDevice->CreateComputeShader(m_pShaderBlob->GetBufferPointer(),
		m_pShaderBlob->GetBufferSize(),
		nullptr,
		&m_pComputeShader),
		"CreateComputeShader");
	return true;
}
