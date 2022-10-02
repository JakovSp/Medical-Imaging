
//
//	Camera.cpp: 
//	Manages the scene fixed camera 
//	
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include "Camera.h"

using namespace std;
using namespace vxe;

using namespace DirectX;

Camera::Camera()
{
	DebugPrint(string("\t Camera::Ctor... \n"));
}

void Camera::Initialize(shared_ptr<VanityCore>& vanitycore)
{
	auto device = vanitycore->GetD3DDevice();

	_view = make_shared<ViewTransform>(device);
	_view->SetView(XMLoadFloat3(&_eye), XMLoadFloat3(&_lookat), XMLoadFloat3(&_up));

	auto size = vanitycore->GetOutputSize();

	auto r = size.Width / size.Height;
	auto fov = 70.0f * XM_PI / 180.0f;
	auto n = 0.01f;
	auto f = 1000.0f;

	if (r < 1.0f) fov *= 2.0f;

	auto _orientation = vanitycore->GetOrientationTransform3D();
	auto orientationMatrix = XMLoadFloat4x4(&_orientation);

	_projection = make_shared<ProjectionTransform>(device);
	_projection->SetProjection(orientationMatrix, fov, r, n, f);
}

void Camera::Bind(shared_ptr<VanityCore>& vanitycore)
{
	auto context = vanitycore->GetD3DDeviceContext();

	_view->Update(context);
	auto buffer = _view->GetConstantBuffer();
	buffer->Bind(context, ProgrammableStage::VertexShaderStage, _viewregister);

	_projection->Update(context);
	_projection->GetConstantBuffer()->Bind(context, ProgrammableStage::VertexShaderStage, _projectionregister);
}

void Camera::Release()
{
	DebugPrint(string("\t Camera::Release()... \n"));

	_view->Reset();
	_projection->Reset();
}