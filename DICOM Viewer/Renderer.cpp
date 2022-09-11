
//
//	Renderer.cpp
//  Implementing a 3D renderer
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include "Renderer.h"
#include "Pipeline.h"

using namespace vxe;
using namespace std;
using namespace concurrency;

using namespace DirectX;
using namespace Windows::Foundation;

Renderer::Renderer() :
	RendererBase3D()
{
	DebugPrint(string("\t Renderer::Ctor... \n"));

	_scene = make_shared<Scene>();
	_pipeline = make_shared<Pipeline>();
}

void Renderer::CreateDeviceResources()
{
	DebugPrint(string("\t Renderer::CreateDeviceResources() ... \n"));

	vector<task<void>> tasks;

	_scene->LoadAssets(tasks, _vanitycore);
	_pipeline->LoadShaders(tasks, _vanitycore);
	
	when_all(tasks.begin(), tasks.end()).then([this]() {
		_loadingcomplete = true;
		DebugPrint(string("\t -- A lambda: Loading is complete! \n"));
	});
}

void Renderer::CreateWindowResources()
{
	DebugPrint(string("\t Renderer::CreateWindowResources() ...\n"));

	_scene->SetCamera(_vanitycore);
}

void Renderer::Render()
{
	if (!_loadingcomplete) return;

	//_pipeline->SetRenderTargets(_vanitycore);
	_pipeline->BindShaders(_vanitycore);

	_scene->Render(_vanitycore);

	_vanitycore->SetRasterizerState();
}

void Renderer::Update(const DX::StepTimer& timer)
{
	_scene->Update(timer);
}

void Renderer::ReleaseDeviceResources()
{
	DebugPrint(string("\t Renderer::ReleaseDeviceResources() ... \n"));

	_loadingcomplete = false;

	_scene->Release();
	_pipeline->Release();
}