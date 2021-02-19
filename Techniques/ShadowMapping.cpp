#include "..\pch.h"
#include "ShadowMapping.h"

ShadowMapping::ShadowMapping() noexcept
    : m_pShaderResourceView{ nullptr },
      m_pDepthStencilViews{ nullptr },
      m_CameraDirections{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
      m_CameraUpVectors{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
      m_SunPosition{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
      m_TextureWidth{ 8192.0f },
      m_TextureHeight{ 8192.0f }
{
    DirectX::XMStoreFloat4x4(&m_OrthographicProjection, DirectX::XMMatrixIdentity());
}

const bool ShadowMapping::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice)
{
    D3D11_TEXTURE2D_DESC textureDescriptor = {};
    textureDescriptor.Width = static_cast<UINT>(m_TextureWidth);  
    textureDescriptor.Height = static_cast<UINT>(m_TextureHeight);
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

    //Constant buffer for holding the shadow matrices:
    D3D11_BUFFER_DESC matrixBufferDescriptor = {};
	matrixBufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDescriptor.ByteWidth = sizeof(MatrixBufferShadow);
	matrixBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDescriptor.MiscFlags = 0;
	matrixBufferDescriptor.StructureByteStride = 0;
	HR(pDevice->CreateBuffer(&matrixBufferDescriptor,
		                     nullptr,
		                     &m_pMatrixCBuffer),
                             "CreateBuffer");

    //We need to tell DXCore to create the proper viewport based on our preferences:
    CreateShadowMapViewportEvent event(m_TextureWidth, m_TextureHeight);
    EventBuss::Get().Delegate(event);

    return true;
}

void ShadowMapping::PreparePasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
    //We start by unbinding the pipeline:
	UnbindPipelineEvent ubEvent;
	EventBuss::Get().Delegate(ubEvent);

    //We need to set the proper viewport on the core:
    SetShadowMapViewportEvent event;
    EventBuss::Get().Delegate(event);

    //We need to set proper Bindables on Resource manager:
    BindIDEvent bindShadowEvent(BindID::ID_Shadow);
    EventBuss::Get().Delegate(bindShadowEvent);
}

void ShadowMapping::DoPasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, const std::vector<GameObject*>* gameObjects) noexcept
{
    /*
    * We need to write depth data in 6 passes, choosing new:
    * Camera directions.
    * Camera up directions.
    * ...and combine the previous two to form the correct camera data.
    * Depth stencil views giving a view on the next 2Dtexture to write to.
    */
    ID3D11RenderTargetView* nullRTV[1] = { nullptr };
    DirectX::XMMATRIX worldMatrix = {};
    for (unsigned int i{ 0u }; i < NUM_PASSES; ++i)
    {
        pDeviceContext->ClearDepthStencilView(m_pDepthStencilViews[i].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
        pDeviceContext->OMSetRenderTargets(1u, nullRTV, m_pDepthStencilViews[i].Get());
        DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_SunPosition), 
                                                                 DirectX::XMLoadFloat3(&m_CameraDirections[i]),   //Will only work for (0,0,0) position! (Emil F)
                                                                 DirectX::XMLoadFloat3(&m_CameraUpVectors[i]));
        for (unsigned int j{ 0u }; j < (*gameObjects).size(); ++j)
        {
            (*gameObjects)[j]->BindShadowUniques(pDeviceContext);
            (*gameObjects)[j]->getWMatrix(worldMatrix);

			D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
			HR_X(pDeviceContext->Map(m_pMatrixCBuffer.Get(),
				                     0,
				                     D3D11_MAP_WRITE_DISCARD,
				                     0,
				                     &mappedSubresource),
				                     "Map");
            MatrixBufferShadow* data = (MatrixBufferShadow*)mappedSubresource.pData;
            data->worldMatrix = worldMatrix;
            data->viewMatrix = viewMatrix;
            data->projectionMatrix = DirectX::XMLoadFloat4x4(&m_OrthographicProjection);
			pDeviceContext->Unmap(m_pMatrixCBuffer.Get(), 0);

            pDeviceContext->DrawIndexed((*gameObjects)[j]->getIndexBufferSize(), 0u, 0);
        }
    }
}

void ShadowMapping::CleanUp() noexcept
{
    //We need to reset the viewport:
    ResetDefaultViewportEvent vpEvent;
    EventBuss::Get().Delegate(vpEvent);
    //DSV and BB is reset in the forward renderer.
    
    //We need to set the cube texture as shader resource!
    
}
