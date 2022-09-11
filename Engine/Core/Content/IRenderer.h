
#pragma once

//
//  IRenderer.h
//
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "../Common/Vanity Core.h"

#include "../Common/StepTimer.h"

namespace vxe {

	struct IRenderer {

		virtual void Initialize(const std::shared_ptr<VanityCore>&) = 0;

		virtual void CreateDeviceResources() = 0;
		virtual void CreateWindowResources() = 0;

		virtual void SetWindow(Windows::UI::Core::CoreWindow^) = 0;

		virtual void Update(DX::StepTimer const&) = 0;
		virtual void Render() = 0;

		virtual void ReleaseDeviceResources() = 0;

		virtual ~IRenderer() = 0;
	};

	inline IRenderer::~IRenderer() {}

}