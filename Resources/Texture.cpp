#include "..\pch.h"
#include "Texture.h"

Texture::Texture() noexcept
    : m_pRenderTargetView{ nullptr },
    m_pShaderResourceView { nullptr },
    m_Slot{ 0u }
{
    
}

const bool Texture::Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, const unsigned int& slot, int width, int height)
{
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(texDesc));
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.SampleDesc.Count = 4u;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    HRESULT hr = pDevice->CreateTexture2D(&texDesc, NULL, &texture);
    if (FAILED(hr)) {
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    rtvDesc.Format = texDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    rtvDesc.Texture2D.MipSlice = 0;

    hr = pDevice->CreateRenderTargetView(texture.Get(), &rtvDesc, m_pRenderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    hr = pDevice->CreateShaderResourceView(texture.Get(), &srvDesc, m_pShaderResourceView.GetAddressOf());
    if (FAILED(hr)) {
        return false;
    }

    return true;
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Texture::getSRV() {
    return m_pShaderResourceView;
}

const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& Texture::getRTV() {
    return m_pRenderTargetView;
}