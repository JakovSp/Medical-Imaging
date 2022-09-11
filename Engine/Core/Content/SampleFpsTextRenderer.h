
#pragma once

//
//	SampleFpsTextRenderer.h
//  Implementing a 2D renderer
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "..\Common\Vanity Core.h"

#include "..\Common\StepTimer.h"

namespace vxe
{
	// Renders the current FPS value in the bottom right corner of the screen using Direct2D and DirectWrite.
	class SampleFpsTextRenderer {

	public:
		SampleFpsTextRenderer(const std::shared_ptr<VanityCore>&);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const&);
		void Render();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<VanityCore> _vanitycore;

		// Resources related to text rendering.
		std::wstring                                    m_text;
		DWRITE_TEXT_METRICS	                            m_textMetrics;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_whiteBrush;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		Microsoft::WRL::ComPtr<IDWriteTextLayout>       m_textLayout;
		Microsoft::WRL::ComPtr<IDWriteTextFormat>		m_textFormat;
	};
}