
//
//	RendererBase3D.cpp
//  Defining a base 3D renderer
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "RendererBase3D.h"

using namespace vxe;
using namespace std;

RendererBase3D::RendererBase3D(InputController^ inputcontroller	) :
	_inputcontroller{ inputcontroller }
{
	DebugPrint(string("\t RendererBase3D::Ctor... \n"));
}

void RendererBase3D::Initialize(const shared_ptr<VanityCore>& vanitycore)
{
	DebugPrint(string("\t RendererBase3D::Initialize()... \n"));

	_vanitycore = vanitycore;

	CreateDeviceResources();
	CreateWindowResources();
}