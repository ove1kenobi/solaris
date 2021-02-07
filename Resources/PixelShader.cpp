#include "..\pch.h"
#include "PixelShader.h"

void PixelShader::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext && m_pPixelShader);
#endif
	pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}

void PixelShader::Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext);
#endif
	pDeviceContext->PSSetShader(nullptr, nullptr, 0u);
}

const bool PixelShader::Create(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, const LPCWSTR& fileName)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{ nullptr };
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	HR_E(D3DCompileFromFile(fileName,
							nullptr,
							D3D_COMPILE_STANDARD_FILE_INCLUDE,
							"ps_main",
							"ps_5_0",
							flags,
							0u,
							&m_pShaderBlob,
							&pErrorBlob),
							pErrorBlob,
							"D3DCompileFromFile");

	HR(pDevice->CreatePixelShader(m_pShaderBlob->GetBufferPointer(),
								  m_pShaderBlob->GetBufferSize(),
								  nullptr,
								  &m_pPixelShader),
								  "CreatePixelShader");
	return true;
}
