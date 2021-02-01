#include "VertexShader.h"
void VertexShader::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext && m_pVertexShader);
#endif
	pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

void VertexShader::Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
{
#if defined(DEBUG) | defined (_DEBUG)
	assert(pDeviceContext);
#endif
	pDeviceContext->VSSetShader(nullptr, nullptr, 0u);
}

const bool VertexShader::Create(Microsoft::WRL::ComPtr<ID3D11Device> pDevice,const LPCWSTR& fileName)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{ nullptr };
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	HR_E(D3DCompileFromFile(fileName,
							nullptr,
							D3D_COMPILE_STANDARD_FILE_INCLUDE,
							"vs_main",
							"vs_5_0",
							flags,
							0u,
							&m_pShaderBlob,
							&pErrorBlob), 
							pErrorBlob, 
							"D3DCompileFromFile");

	HR(pDevice->CreateVertexShader(m_pShaderBlob->GetBufferPointer(), 
								   m_pShaderBlob->GetBufferSize(),
								   nullptr, 
								   &m_pVertexShader), 
								   "CreateVertexShader");
	return true;
}

const Microsoft::WRL::ComPtr<ID3DBlob>& VertexShader::GetVertexShaderBlob() const noexcept
{
	return m_pShaderBlob;
}
