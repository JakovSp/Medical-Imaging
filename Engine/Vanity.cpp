
//
//	Vanity.cpp: The application's entry point
//
//	Objectives:
//	(1) The constructor creates the application view factory
//	(2) Method Run passes the factory to WinRT
//	(3) WinRT calls methods in the application view 
// 
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Vanity.h"
#include "Core/Application Factory.h"

using namespace Windows::ApplicationModel::Core;
using namespace vxe;
using namespace std;

Vanity::Vanity(shared_ptr<IRenderer> renderer)
{
	_factory = ref new ApplicationFactory(renderer);
}

void Vanity::Run()
{
	CoreApplication::Run(_factory);
}