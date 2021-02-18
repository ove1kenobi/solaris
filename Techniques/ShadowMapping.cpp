#include "..\pch.h"
#include "ShadowMapping.h"

ShadowMapping::ShadowMapping() noexcept
    : m_pShaderResourceView{ nullptr },
      m_pDepthStencilViews{ nullptr },
      m_CameraDirections{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
      m_CameraUpVectors{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
      m_TextureWidth{ 8192u },
      m_TextureHeight{ 8192u }
{
    DirectX::XMStoreFloat4x4(&m_OrthographicProjection, DirectX::XMMatrixIdentity());
}

const bool ShadowMapping::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice)
{
    D3D11_TEXTURE2D_DESC textureDescriptor = {};
    textureDescriptor.Width = m_TextureWidth;  
    textureDescriptor.Height = m_TextureHeight;
    textureDescriptor.MipLevels = 1u;
    textureDescriptor.ArraySize = 6u;
    textureDescriptor.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
    textureDescriptor.SampleDesc.Count = 1u; //WARNING MSAA4x
    textureDescriptor.SampleDesc.Quality = 0u; //-*-
    textureDescriptor.Usage = D3D11_USAGE_DEFAULT;
    textureDescriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
    textureDescriptor.CPUAccessFlags = 0u;
    textureDescriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture{ nullptr };
    HR(pDevice->CreateTexture2D(&textureDescriptor, 
                                nullptr, 
                                &pTexture), 
                                "CreateTexture2D");
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescriptor = {};
    shaderResourceViewDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
    shaderResourceViewDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    shaderResourceViewDescriptor.Texture2D.MostDetailedMip = 0u;
    shaderResourceViewDescriptor.Texture2D.MipLevels = 1u;
    HR(pDevice->CreateShaderResourceView(pTexture.Get(), 
                                         &shaderResourceViewDescriptor, 
                                         &m_pShaderResourceView), 
                                         "CreateShaderResourceView");
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor = {};
    depthStencilViewDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDescriptor.Flags = 0u;
    depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    depthStencilViewDescriptor.Texture2DArray.MipSlice = 0u;
    depthStencilViewDescriptor.Texture2DArray.ArraySize = 1u;
    for (unsigned int i{ 0u }; i < NUM_PASSES; ++i)
    {
        depthStencilViewDescriptor.Texture2DArray.FirstArraySlice = i;
        HR(pDevice->CreateDepthStencilView(pTexture.Get(), 
                                           &depthStencilViewDescriptor, 
                                           &m_pDepthStencilViews[i]), 
                                           "CreateDepthStencilView");
    }

    //Currently taking in the same far plane value as the player camera has (10000):
    DirectX::XMStoreFloat4x4(&m_OrthographicProjection, DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 10000.0f));

    //The different directions:
    //+X
    m_CameraDirections[0]   = { 1.0f, 0.0f, 0.0f };
    m_CameraUpVectors[0]    = { 0.0f, 1.0f, 0.0f };
    //-X
    m_CameraDirections[1]   = { -1.0f, 0.0f, 0.0f };
    m_CameraUpVectors[1]    = { 0.0f, 1.0f, 0.0f };
    //+Y
    m_CameraDirections[2]   = { 0.0f, 1.0f, 0.0f };
    m_CameraUpVectors[2]    = { 0.0f, 0.0f, -1.0f };
    //-Y
    m_CameraDirections[3]   = { 0.0f, -1.0f, 0.0f };
    m_CameraUpVectors[3]    = { 0.0f, 0.0f, 1.0f };
    //+Z
    m_CameraDirections[4]   = { 0.0f, 0.0f, 1.0f };
    m_CameraUpVectors[4]    = { 0.0f, 1.0f, 0.0f };
    //-Z
    m_CameraDirections[5]   = { 0.0f, 0.0f, -1.0f };
    m_CameraUpVectors[5]    = { 0.0f, 1.0f, 0.0f };
    return true;
}

void ShadowMapping::PreparePasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
    //We need to set a proper viewport!

}

void ShadowMapping::DoPasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
    /*
    * We need to write depth data in 6 passes, choosing new:
    * Camera directions.
    * Camera up directions.
    * ...and combine the previous two to form the correct camera data.
    * Depth stencil view giving a view on the next 2Dtexture to write to.
    */
   
}

void ShadowMapping::CleanUp() noexcept
{
    //We need to reset the viewport!

    //We need to reset the DSV!

    //We need to set the cube texture as shader resource!

}
