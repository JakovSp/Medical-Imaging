
#pragma once

//
//	Application Factory.h
//  Defining the aplication factory
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "App.h"
#include "Content\RendererBase3D.h"

namespace vxe {

	ref class ApplicationFactory sealed : Windows::ApplicationModel::Core::IFrameworkViewSource{

	internal:
		App^ _application;
		std::shared_ptr<IRenderer> _renderer3D;

		//The public surface area of a WinRT type can only contain WinRT compatible types
		ApplicationFactory(const std::shared_ptr<IRenderer>& renderer3D)
			: _renderer3D{ renderer3D }
		{
			DebugPrint(std::string("ApplicationFactory::Ctor ...\n"));
		}

	public:
		virtual Windows::ApplicationModel::Core::IFrameworkView ^CreateView()
		{
			DebugPrint(std::string("ApplicationFactory::CreateView() ...\n"));

			_application = ref new App(_renderer3D);

			return _application;
		}

		virtual ~ApplicationFactory()
		{
			DebugPrint(std::string("ApplicationFactory::Dtor ...\n"));
		}
	};
}