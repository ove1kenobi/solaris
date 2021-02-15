#include "..\pch.h"
#include "Texture.h"

Texture::Texture() noexcept
    : m_pRenderTargetView{ nullptr },
    m_pShaderResourceView { nullptr },
    m_pUnorderedAccessView{ nullptr }, 
    m_pDepthStencilView{ nullptr },
    m_Slot{ 0u }, m_isRenderTarget{ false },
    m_background{0.5f, 0.5f, 0.5f, 1.0f}
{
    
}

void Texture::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
    assert(pDeviceContext && m_pShaderResourceView);
#endif

    if (m_isRenderTarget) {
        pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
        pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), m_background);
        m_isRenderTarget = false;
    }
    else {
        pDeviceContext->PSSetShaderResources(m_Slot, 1u, m_pShaderResourceView.GetAddressOf());
        if (m_pRenderTargetView) {
            m_isRenderTarget = true;
        }
    }
   
    if (m_pUnorderedAccessView) {
        pDeviceContext->CSSetUnorderedAccessViews(0, 1, m_pUnorderedAccessView.GetAddressOf(), NULL);
    }
}

void Texture::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
    assert(pDeviceContext);
#endif
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    pDeviceContext->PSSetShaderResources(m_Slot, 1u, nullSRV);

    if (m_pRenderTargetView) {
        ID3D11RenderTargetView* nullRTV[1] = { nullptr };
        pDeviceContext->OMSetRenderTargets(1, nullRTV, m_pDepthStencilView.Get());
    }

    if (m_pUnorderedAccessView) {
        ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
        pDeviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, NULL);
    }
}

const bool Texture::Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const unsigned int& slot, int width, int height, D3D11_USAGE usage, UINT bindflags, UINT cpuaccessflags)
{
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(texDesc));
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.SampleDesc.Count = 4u;
    texDesc.Usage = usage;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | bindflags;
    texDesc.CPUAccessFlags = cpuaccessflags;
    texDesc.MiscFlags = 0;

    ID3D11Texture2D* texture;
    HRESULT hr = pDevice->CreateTexture2D(&texDesc, NULL, &texture);
    if (FAILED(hr)) {
        return false;
    }

    if(bindflags & D3D11_BIND_RENDER_TARGET){
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = texDesc.Format;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
        rtvDesc.Texture2D.MipSlice = 0;

        hr = pDevice->CreateRenderTargetView(texture, &rtvDesc, m_pRenderTargetView.GetAddressOf());
        if (FAILED(hr)) {
            return false;
        }
        
        EventBuss::Get().AddListener(this, EventType::SendDSVEvent);

        RequestDSVEvent event;
        EventBuss::Get().Delegate(event);

        m_isRenderTarget = true;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    hr = pDevice->CreateShaderResourceView(texture, &srvDesc, m_pShaderResourceView.GetAddressOf());
    if (FAILED(hr)) {
        return false;
    }

    if (bindflags & D3D11_BIND_UNORDERED_ACCESS) {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
        uavDesc.Format = texDesc.Format;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
        uavDesc.Texture2D.MipSlice = 0;

        hr = pDevice->CreateUnorderedAccessView(texture, &uavDesc, m_pUnorderedAccessView.GetAddressOf());
        if (FAILED(hr)) {
            return false;
        }
    }

    return true;
}

void Texture::OnEvent(IEvent& event) noexcept {
    switch (event.GetEventType()) {
    case EventType::SendDSVEvent:
    {
        SendDSVEvent& derivedEvent = static_cast<SendDSVEvent&>(event);
        m_pDepthStencilView = derivedEvent.GetDepthStencilView();
        break;
    }
    }
}