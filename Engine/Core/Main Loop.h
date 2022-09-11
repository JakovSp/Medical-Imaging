
#pragma once

//
//	Pipeline.h
//  Managing DirectX 11.2 components (the pipeline and the renderers)
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Common\Vanity Core.h"

#include "Common\StepTimer.h"
#include "Content\RendererBase3D.h"
#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace vxe
{
	class MainLoop : public IDeviceNotify {

	public:
		MainLoop(const std::shared_ptr<VanityCore>&, const std::shared_ptr<IRenderer>&);

		~MainLoop();

		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<VanityCore> _vanitycore;

		// TODO: Replace with your own content renderers.
		std::shared_ptr<IRenderer> _sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> _fpsTextRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;
	};
}