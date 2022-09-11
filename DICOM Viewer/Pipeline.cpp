
//
//	Pipeline.cpp: 
//	Manages pipeline stages 
//	
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include <Engine\Models\Basic Shapes\Cubes.h>

#include "Pipeline.h"

using namespace std;
using namespace vxe;

using namespace concurrency;
using namespace DirectX;

Pipeline::Pipeline()
{
	DebugPrint(std::string("\t Pipeline::Ctor... \n"));
}

void Pipeline::LoadShaders(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore)
{
	DebugPrint(string("\t Pipeline::LoadShaders() ... \n"));

	auto device = vanitycore->GetD3DDevice();
	
	_vertexshader = make_shared<VertexShader<VertexType_t>>();
	tasks.push_back(_vertexshader->CreateAsync(device, _vsfilename));

	_pixelshader = make_shared<PixelShader>();
	tasks.push_back(_pixelshader->CreateAsync(device, _psfilename));
}

void Pipeline::BindShaders(shared_ptr<VanityCore>& vanitycore)
{
	auto context = vanitycore->GetD3DDeviceContext();

	_vertexshader->Bind(context);
	_pixelshader->Bind(context);
}

// already set in MainLoop::Render() 
//void Pipeline::SetRenderTargets(shared_ptr<VanityCore>& vanitycore)
//{
//	ID3D11RenderTargetView* const rtvs[1] = { vanitycore->GetBackBufferRenderTargetView() };
//	auto dsv = vanitycore->GetDepthStencilView();
//
//	auto context = vanitycore->GetD3DDeviceContext();
//	context->OMSetRenderTargets(1, rtvs, dsv);
//}

void Pipeline::Release()
{
	DebugPrint(string("\t Pipeline::Release() ... \n"));

	_vertexshader->Reset();
	_pixelshader->Reset();
}