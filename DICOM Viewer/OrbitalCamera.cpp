
//
//	OrbitalCamera.cpp: 
//	Manages the scene fixed camera 
//	
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include "OrbitalCamera.h"

using namespace std;
using namespace vxe;

using namespace DirectX;

OrbitalCamera::OrbitalCamera()
{
	DebugPrint(string("\t Camera::Ctor... \n"));
}

OrbitalCamera::OrbitalCamera(float distance) :
	_distance(distance), _eye{0.0f, 0.0f, distance}
{
	DebugPrint(string("\t Camera::Ctor... \n"));
}

void OrbitalCamera::Initialize(shared_ptr<VanityCore>& vanitycore)
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

	auto orientation = vanitycore->GetOrientationTransform3D();
	auto orientationMatrix = XMLoadFloat4x4(&orientation);

	_projection = make_shared<ProjectionTransform>(device);
	_projection->SetProjection(orientationMatrix, fov, r, n, f);
}

void OrbitalCamera::Bind(shared_ptr<VanityCore>& vanitycore)
{
	auto context = vanitycore->GetD3DDeviceContext();

	_view->Update(context);
	auto buffer = _view->GetConstantBuffer();
	buffer->Bind(context, ProgrammableStage::VertexShaderStage, _viewregister);

	_projection->Update(context);
	_projection->GetConstantBuffer()->Bind(context, ProgrammableStage::VertexShaderStage, _projectionregister);
}

void OrbitalCamera::Update(shared_ptr<VanityCore>& vc, const float& dz, const float& dx, const float& dy) {
	_distance += dz; _alpha += dx; _beta += dy;
	_eye.x = cos(_alpha) * _distance * cos(_beta) + _lookat.x;
	_eye.z = sin(_alpha) * _distance * cos(_beta) + _lookat.z;
	_eye.y = sin(_beta) * _distance + _lookat.y;

	_view->SetView(XMLoadFloat3(&_eye), XMLoadFloat3(&_lookat), XMLoadFloat3(&_up));
	auto context = vc->GetD3DDeviceContext();
	_view->Update(context);
}

void OrbitalCamera::Release()
{
	DebugPrint(string("\t Camera::Release()... \n"));

	_view->Reset();
	_projection->Reset();
}