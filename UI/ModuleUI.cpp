#include "..\pch.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI() noexcept {
	EventBuss::Get().AddListener(this, EventType::DelegateDXEvent);
	EventBuss::Get().AddListener(this, EventType::DelegateResolutionEvent);
	m_pWindowWidth = 0.0f;
	m_pWindowHeight = 0.0f;
}

bool ModuleUI::CreateBrush() {
	return ErrorCheck(m_pRenderTarget2D->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Aqua, 0.5f),
		&m_pBrush),
		"SolidColorBrush"
	);
}

std::wstring ModuleUI::GetIconFilePath(std::wstring iconFile) {
	std::wstring FilePath;

	//Get current directory
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	FilePath.append(NPath);

	//Get icon file
	FilePath.append(L"\\UI\\Icons\\");
	FilePath.append(iconFile);

	return FilePath;
}

void ModuleUI::LoadBitmapFromFile(PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap) {
	//Encode picture from filepath into WICmipmap
	ErrorCheck(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pBitMapFactory)), "CreateInstance");
	ErrorCheck(m_pBitMapFactory->CreateDecoderFromFilename(uri, &GUID_ContainerFormatPng, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_pBitMapDecoder), "CreateDecoderFromFilename");
	
	//Get first frame of picture (has to be done in case it is a gif).
	ErrorCheck(m_pBitMapDecoder->GetFrame(0, &m_pBitMapSource), "GetFrame");

	//Convert the image format to 32bppPBGRA as that what direct2D bitmaps are in
	ErrorCheck(m_pBitMapFactory->CreateFormatConverter(&m_pBitMapConvert), "CreateFormatConverter");
	ErrorCheck(m_pBitMapConvert->Initialize(m_pBitMapSource.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut), "pConverter");

	//Create a Direct2D bitmap from the WIC bitmap.
	ErrorCheck(m_pRenderTarget2D->CreateBitmapFromWicBitmap(m_pBitMapConvert.Get(), NULL, ppBitmap), "CreateBitmapFromWicBitmap");
}

void ModuleUI::UpdateDXHandlers(IEvent& event) noexcept {
	DelegateDXEvent& derivedEvent = static_cast<DelegateDXEvent&>(event);
	
	m_pFactory2D = derivedEvent.GetFactory2D();
	m_pRenderTarget2D = derivedEvent.GetSurfaceRenderTarget();
	m_pTextFactory = derivedEvent.GetTextFactory();

	#if defined(DEBUG) | defined(_DEBUG)
	assert(m_pFactory2D && m_pRenderTarget2D && m_pTextFactory);
	#endif
}

bool ModuleUI::ErrorCheck(HRESULT handle, std::string type) {
	std::string error;
	if (FAILED(handle)) {
		error.append("Error: ");
		error.append(type);
		error.append(" couldn't be created.");
		printf(error.c_str());
		return false;
	}
	return true;
}

void ModuleUI::UpdateBrush(D2D1::ColorF color, float opacity) {
	this->m_pBrush.Get()->SetColor(color);
	this->m_pBrush.Get()->SetOpacity(opacity);
}

void ModuleUI::BeginFrame() {
	m_pRenderTarget2D->BeginDraw();
}

void ModuleUI::RenderHelpGrid(int gridSize) {
	this->UpdateBrush(D2D1::ColorF::Aqua, 0.5f);
	for (unsigned int x = 0; x < m_pWindowWidth; x += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), static_cast<float>(m_pWindowHeight)),
			m_pBrush.Get(),
			0.5f
		);
	}

	for (unsigned int y = 0; y < m_pWindowHeight; y += gridSize)
	{
		m_pRenderTarget2D->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(static_cast<float>(m_pWindowWidth), static_cast<FLOAT>(y)),
			m_pBrush.Get(),
			0.5f
		);
	}
}

void ModuleUI::EndFrame() {
	ErrorCheck(m_pRenderTarget2D->EndDraw(), "EndDraw");
}
