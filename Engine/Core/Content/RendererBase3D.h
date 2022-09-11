
#pragma once

//
//	RendererBase3D.h
//  Defining a base 3D renderer
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\Common\Vanity Core.h"

#include "..\..\Utilities.h"
#include "..\Common\StepTimer.h"
#include "..\..\Scene\Input\Input Controller.h"

#include "IRenderer.h"

namespace vxe {

	class RendererBase3D : public IRenderer {

	public:
		RendererBase3D(InputController^ = nullptr);

		void Initialize(const std::shared_ptr<VanityCore>&);

		virtual void TrackingUpdate(float) {};

		void virtual SetWindow(Windows::UI::Core::CoreWindow^ window) override
		{
			if(_inputcontroller != nullptr) _inputcontroller->Initialize(window);
		}

		~RendererBase3D() {};

	protected:
		std::shared_ptr<VanityCore> _vanitycore;
		InputController^ _inputcontroller;

		bool _loadingcomplete{false};
	};

}