
#pragma once

//
//	Vanity.h: The application's entry point
// 
//	This version is based on Visual Studio 2019 DirextX11 templates.
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Core/Content/IRenderer.h"

namespace vxe {

	class Vanity {

	public:
		explicit Vanity(std::shared_ptr<IRenderer> = nullptr);

		void Run();

	private:
		Windows::ApplicationModel::Core::IFrameworkViewSource^ _factory = nullptr;
	};

}