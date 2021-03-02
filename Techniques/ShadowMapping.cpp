#include "..\pch.h"
#include "ShadowMapping.h"

ShadowMapping::ShadowMapping() noexcept
    : m_pShaderResourceView{ nullptr },
      m_pDepthStencilViews{ nullptr },
      m_pRenderTargetViews{ nullptr },
      m_pMatrixCBuffer{ nullptr },
      m_pShadowDataBuffer{ nullptr },
      m_pLightDataBuffer{ nullptr },
      m_pShadowBlendState{ nullptr },
      m_pDefaultBlendState{ nullptr },
      m_CameraDirections{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
      m_CameraUpVectors{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
      m_SunPosition{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
      m_TextureWidth{ 6144.0f },
      m_TextureHeight{ 6144.0f },
      m_ClearColor{ 1.0f, 0.0f, 0.0f, 1.0f },
      m_ShadowBias{ 20.0f }
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
    textureDescriptor.SampleDesc.Count = 1u;
    textureDescriptor.SampleDesc.Quality = 0u;
    textureDescriptor.Usage = D3D11_USAGE_DEFAULT;
    textureDescriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
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
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescriptor = {};
    renderTargetViewDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
    renderTargetViewDescriptor.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    renderTargetViewDescriptor.Texture2DArray.MipSlice = 0u;
    renderTargetViewDescriptor.Texture2DArray.ArraySize = 1u;
    for (unsigned int i{ 0u }; i < NUM_PASSES; ++i)
       {
           renderTargetViewDescriptor.Texture2DArray.FirstArraySlice = i;
           HR(pDevice->CreateRenderTargetView(pTexture.Get(), 
			                                  &renderTargetViewDescriptor,
			                                  &m_pRenderTargetViews[i]),
			                                  "CreateRenderTargetView");
       }
    //Currently taking in the same far plane value as the player camera has (100000):
    DirectX::XMStoreFloat4x4(&m_OrthographicProjection, DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 100000.0f));
    //We create the "shadow bounding frustum":
    m_ShadowFrustumCulling.CreateShadowFrustum(DirectX::XMLoadFloat4x4(&m_OrthographicProjection));
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

	//Constant buffer for holding the shadow data:
	D3D11_BUFFER_DESC shadowDataBufferDescriptor = {};
    shadowDataBufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
    shadowDataBufferDescriptor.ByteWidth = sizeof(ShadowData);
    shadowDataBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    shadowDataBufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    shadowDataBufferDescriptor.MiscFlags = 0;
    shadowDataBufferDescriptor.StructureByteStride = 0;
	HR(pDevice->CreateBuffer(&shadowDataBufferDescriptor,
		                     nullptr,
		                     &m_pShadowDataBuffer),
		                     "CreateBuffer");

	//Constant buffer for holding the shadow data:
	D3D11_BUFFER_DESC lightDataBufferDescriptor = {};
    lightDataBufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
    lightDataBufferDescriptor.ByteWidth = sizeof(ShadowData);
    lightDataBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightDataBufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightDataBufferDescriptor.MiscFlags = 0;
    lightDataBufferDescriptor.StructureByteStride = 0;
	HR(pDevice->CreateBuffer(&lightDataBufferDescriptor,
		                    nullptr,
		                    &m_pLightDataBuffer),
		                    "CreateBuffer");

    //We need to tell DXCore to create the proper viewport based on our preferences:
    CreateShadowMapViewportEvent event(m_TextureWidth, m_TextureHeight);
    EventBuss::Get().Delegate(event);

    //Create Shadow blend state:
    D3D11_BLEND_DESC blendDescriptor = {};
    blendDescriptor.AlphaToCoverageEnable = FALSE;
    blendDescriptor.IndependentBlendEnable = FALSE;
    blendDescriptor.RenderTarget[0].BlendEnable = TRUE;
    blendDescriptor.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDescriptor.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    blendDescriptor.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;
    blendDescriptor.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDescriptor.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDescriptor.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;
    blendDescriptor.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    HR(pDevice->CreateBlendState(&blendDescriptor, &m_pShadowBlendState), "CreateBlendState");

    //Create default blend state:
    blendDescriptor.RenderTarget[0].BlendEnable = FALSE;
    blendDescriptor.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDescriptor.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    HR(pDevice->CreateBlendState(&blendDescriptor, &m_pDefaultBlendState), "CreateBlendState");
    return true;
}

void ShadowMapping::PreparePasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, const DirectX::XMFLOAT3& lightPosition) noexcept
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

    //We update the light position in shader:
    m_SunPosition = lightPosition;
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	HR_X(pDeviceContext->Map(m_pLightDataBuffer.Get(),
		                       0,
		                       D3D11_MAP_WRITE_DISCARD,
		                       0,
		                       &mappedSubresource),
		                       "Map");
    LightData* data = (LightData*)mappedSubresource.pData;
    data->position = lightPosition;
	pDeviceContext->Unmap(m_pLightDataBuffer.Get(), 0);
	pDeviceContext->PSSetConstantBuffers(0u, 1u, m_pLightDataBuffer.GetAddressOf());

    //Set shadow blend state:
    SetShadowBlendStateEvent shadowBlendEvent;
    EventBuss::Get().Delegate(shadowBlendEvent);
}

void ShadowMapping::DoPasses(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, const RenderData* pRenderData) noexcept
{
    /*
    * We need to write depth data in 6 passes, choosing new:
    * Camera directions.
    * Camera up directions.
    * ...and combine the previous two to form the correct camera data.
    * Render target views giving a view on the next 2Dtexture to write to.
    */
    DirectX::XMMATRIX worldMatrix = {};
    ID3D11DepthStencilView* nullDSV = nullptr;
    for (unsigned int i{ 0u }; i < NUM_PASSES; ++i)
    {
        pDeviceContext->ClearRenderTargetView(m_pRenderTargetViews[i].Get(), m_ClearColor);
        pDeviceContext->OMSetRenderTargets(1u, m_pRenderTargetViews[i].GetAddressOf(), nullDSV);
        const DirectX::XMVECTOR focusPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_SunPosition), DirectX::XMLoadFloat3(&m_CameraDirections[i]));
        DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_SunPosition),
            focusPosition,
            DirectX::XMLoadFloat3(&m_CameraUpVectors[i]));
        //Do shadow frustum culling based on current view matrix:
        std::vector<GameObject*> culledObjects = m_ShadowFrustumCulling.CullShadowObjects(pRenderData->allObjects, viewMatrix, pRenderData->totalNrOfPlanets);

        for (size_t j{ 0 }; j < culledObjects.size(); ++j)
        {
            culledObjects[j]->BindShadowUniques(pDeviceContext);
            culledObjects[j]->getWMatrix(worldMatrix);

            D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
            HR_X(pDeviceContext->Map(m_pMatrixCBuffer.Get(),
                                     0,
                                     D3D11_MAP_WRITE_DISCARD,
                                     0,
                                     &mappedSubresource),
                                     "Map");
            MatrixBufferShadow* data = (MatrixBufferShadow*)mappedSubresource.pData;
            data->worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
            data->worldViewProjectionMatrix = DirectX::XMMatrixTranspose(worldMatrix * viewMatrix * DirectX::XMLoadFloat4x4(&m_OrthographicProjection));
            pDeviceContext->Unmap(m_pMatrixCBuffer.Get(), 0);
            pDeviceContext->VSSetConstantBuffers(0u, 1u, m_pMatrixCBuffer.GetAddressOf());
            pDeviceContext->DrawIndexed(culledObjects[j]->getIndexBufferSize(), 0u, 0);
        }
    }
}

void ShadowMapping::CleanUp(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
    //We need to reset the viewport:
    ResetDefaultViewportEvent vpEvent;
    EventBuss::Get().Delegate(vpEvent);

    //We reset the blend state:
    ResetDefaultBlendStateEvent resetBlendEvent;
    EventBuss::Get().Delegate(resetBlendEvent);
}

void ShadowMapping::BindSRV(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
    pDeviceContext->PSSetShaderResources(4u, 1u, m_pShaderResourceView.GetAddressOf());
}

void ShadowMapping::UpdateBias(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) noexcept
{
#if defined(DEBUG) | defined(_DEBUG)
    ImGui::Begin("Shadow");
    ImGui::DragFloat("Bias", &m_ShadowBias, 0.001f, 0.0f, 40.0f);
    ImGui::End();
#endif
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	HR_X(pDeviceContext->Map(m_pShadowDataBuffer.Get(),
		                     0,
		                     D3D11_MAP_WRITE_DISCARD,
		                     0,
		                     &mappedSubresource),
		                     "Map");
    ShadowData* data = (ShadowData*)mappedSubresource.pData;
    data->bias = m_ShadowBias;
	pDeviceContext->Unmap(m_pShadowDataBuffer.Get(), 0);
	pDeviceContext->PSSetConstantBuffers(4u, 1u, m_pShadowDataBuffer.GetAddressOf());
}
