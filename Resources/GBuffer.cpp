#include "..\pch.h"
#include "GBuffer.h"

GBuffer::GBuffer() noexcept
    : 
    m_pDepthStencilView{ nullptr },
    m_isRenderTarget{ false },
    m_background{ 0.5f, 0.5f, 0.5f, 0.5f },
    m_RTVArray{ nullptr }
{

}

GBuffer::~GBuffer()
{
    for (int i = 0; i < m_Textures.size(); i++) {
        delete m_Textures[i];
    }
}

void GBuffer::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
    assert(pDeviceContext);
#endif

    if (m_isRenderTarget) {
        pDeviceContext->OMSetRenderTargets(ARRAYSIZE(m_RTVArray), m_RTVArray->GetAddressOf(), m_pDepthStencilView.Get());
        for (int i = 0; i < ARRAYSIZE(m_RTVArray); i++) {
            pDeviceContext->ClearRenderTargetView(m_RTVArray[i].Get(), m_background);
        }
        m_isRenderTarget = false;
    }
    else {
        for (int i = 0; i < m_Textures.size(); i++) {
            pDeviceContext->PSSetShaderResources(i, 1u, m_Textures[i]->getSRV().GetAddressOf());
        }
        
        m_isRenderTarget = true;
    }
}

void GBuffer::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext)
{
#if defined(DEBUG) | defined(_DEBUG)
    assert(pDeviceContext);
#endif
    ID3D11ShaderResourceView* nullSRV[nr] = { nullptr };
    ID3D11RenderTargetView* nullRTV[nr] = { nullptr };
    pDeviceContext->PSSetShaderResources(0u, (UINT)m_Textures.size(), nullSRV);
    pDeviceContext->OMSetRenderTargets(ARRAYSIZE(m_RTVArray), nullRTV, m_pDepthStencilView.Get());
}

const bool GBuffer::Create(const Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, int width, int height)
{
    Texture* colorTexture = new Texture();
    if (!colorTexture->Create(pDevice, 0, width, height))
        return false;
    m_Textures.push_back(colorTexture);

    Texture* waterBoolTexture = new Texture();
    if (!waterBoolTexture->Create(pDevice, 1, width, height))
        return false;
    m_Textures.push_back(waterBoolTexture);

    Texture* lengthCenterTexture = new Texture();
    if (!lengthCenterTexture->Create(pDevice, 2, width, height))
        return false;
    m_Textures.push_back(lengthCenterTexture);

    Texture* wPosTexture = new Texture();
    if (!wPosTexture->Create(pDevice, 3, width, height))
        return false;
    m_Textures.push_back(wPosTexture);

    Texture* normalTexture = new Texture();
    if (!normalTexture->Create(pDevice, 4, width, height))
        return false;
    m_Textures.push_back(normalTexture);

    Texture* waterSphereTexture = new Texture();
    if (!waterSphereTexture->Create(pDevice, 5, width, height))
        return false;
    m_Textures.push_back(waterSphereTexture);

    for (int i = 0; i < ARRAYSIZE(m_RTVArray); i++) {
        m_RTVArray[i] = m_Textures[i]->getRTV();
    }
    m_isRenderTarget = true;

    EventBuss::Get().AddListener(this, EventType::SendDSVEvent);

    RequestDSVEvent event;
    EventBuss::Get().Delegate(event);

    return true;
}

void GBuffer::OnEvent(IEvent& event) noexcept {
    switch (event.GetEventType()) {
    case EventType::SendDSVEvent:
    {
        SendDSVEvent& derivedEvent = static_cast<SendDSVEvent&>(event);
        m_pDepthStencilView = derivedEvent.GetDepthStencilView();
        break;
    }
    }
}