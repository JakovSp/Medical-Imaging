
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

	_inputcontroller = ref new InputController();
	_scene = make_shared<Scene>();
	_volumetricpipeline = make_shared<Pipeline<VertexPositionTexture3>>(
		L"VolumetricVS.cso", L"VolumetricPS.cso");
	_wireframepipeline = make_shared<Pipeline<VertexPosition>>(
		L"WireframeVS.cso", L"WireframePS.cso");
}

void Renderer::CreateDeviceResources()
{
	DebugPrint(string("\t Renderer::CreateDeviceResources() ... \n"));

	vector<task<void>> tasks;

	_scene->LoadAssets(tasks, _vanitycore);
	_volumetricpipeline->LoadShaders(tasks, _vanitycore);
	_wireframepipeline->LoadShaders(tasks, _vanitycore);
	
	when_all(tasks.begin(), tasks.end()).then([this]() {
		_loadingcomplete = true;
		DebugPrint(string("\t -- A lambda: Loading is complete! \n"));

		_scene->SetTextures(_vanitycore);
	});

	_objectbound = Volumetric;
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
	switch (_objectbound) {
	case TriMesh:
		_wireframepipeline->BindShaders(_vanitycore);
		_scene->DrawTriMesh(_vanitycore);
		break;
	case PointCloud:
		_wireframepipeline->BindShaders(_vanitycore);
		_scene->DrawPointCloud(_vanitycore);
		break;
	case Volumetric:
		_volumetricpipeline->BindShaders(_vanitycore);
		_scene->DrawVolumetric(_vanitycore);
		break;
	}
	_vanitycore->SetRasterizerState();
}

void Renderer::Update(const DX::StepTimer& timer)
{
	if (_inputcontroller->IsKeyDown(Windows::System::VirtualKey::Space)) {
		_objectbound = (SceneObjectType)((_objectbound + 1 ) % NumberOfObjects);
		_inputcontroller->ResetSates();
	}
	_scene->Update(timer, _vanitycore, _inputcontroller);
}


void Renderer::ReleaseDeviceResources()
{
	DebugPrint(string("\t Renderer::ReleaseDeviceResources() ... \n"));

	_loadingcomplete = false;

	_scene->Release();
	switch (_objectbound) {
	case PointCloud: TriMesh:
		_volumetricpipeline->Release();
		break;
	case Volumetric:
		_wireframepipeline->Release();
		break;
	}
}