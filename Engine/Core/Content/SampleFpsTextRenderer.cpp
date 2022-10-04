
//
//	SampleFpsTextRenderer.cpp
//  Implementing a 2D renderer
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\..\Utilities.h"
#include "SampleFpsTextRenderer.h"
#include "../Common/DirectXHelper.h"

using namespace vxe;

// Initializes D2D resources used for text rendering.
SampleFpsTextRenderer::SampleFpsTextRenderer(const std::shared_ptr<VanityCore>& vanitycore) : 
	m_text(L""),
	_vanitycore(vanitycore)
{
	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

	// Create device independent resources
	ThrowIfFailed(
		_vanitycore->GetDWriteFactory()->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"en-US",
		&m_textFormat
		), __FILEW__, __LINE__);

	ThrowIfFailed(m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR), __FILEW__, __LINE__);

	ThrowIfFailed(_vanitycore->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock), __FILEW__, __LINE__);

	CreateDeviceDependentResources();
}

// Updates the text to be displayed.
void SampleFpsTextRenderer::Update(DX::StepTimer const& timer)
{
	// Update display text.
	uint32 fps = timer.GetFramesPerSecond();

	m_text = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";

	ThrowIfFailed(
		_vanitycore->GetDWriteFactory()->CreateTextLayout(
		m_text.c_str(),
		(uint32) m_text.length(),
		m_textFormat.Get(),
		240.0f, // Max samples_x of the input text.
		50.0f, // Max height of the input text.
		&m_textLayout), __FILEW__, __LINE__);

	ThrowIfFailed(m_textLayout->GetMetrics(&m_textMetrics), __FILEW__, __LINE__);
}

// Renders a frame to the screen.
void SampleFpsTextRenderer::Render()
{
	ID2D1DeviceContext* context = _vanitycore->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = _vanitycore->GetLogicalSize();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	// Position on the bottom right corner
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
		logicalSize.Width - m_textMetrics.layoutWidth,
		logicalSize.Height - m_textMetrics.height
		);

	context->SetTransform(screenTranslation * _vanitycore->GetOrientationTransform2D());

	ThrowIfFailed(m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING), __FILEW__, __LINE__);

	context->DrawTextLayout(
		D2D1::Point2F(0.f, 0.f),
		m_textLayout.Get(),
		m_whiteBrush.Get()
		);

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		ThrowIfFailed(hr, __FILEW__, __LINE__);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void SampleFpsTextRenderer::CreateDeviceDependentResources()
{
	ThrowIfFailed(_vanitycore->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_whiteBrush), __FILEW__, __LINE__);
}

void SampleFpsTextRenderer::ReleaseDeviceDependentResources()
{
	m_whiteBrush.Reset();
}