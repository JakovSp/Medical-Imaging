
//
//	Camera Base.cpp
//	Encapsulates basic camera elements: position, view and projection
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Camera Base.h"

using namespace vxe;
using namespace std;
using namespace DirectX;

CameraBase::CameraBase(_In_ ID3D11Device2* device) :
	_eye{ device },
	_view{ device },
	_projection{ device }
{}

void CameraBase::InitializeView(FXMVECTOR eye, FXMVECTOR lookat, FXMVECTOR up)
{
	DebugPrint(string("\t CameraBase::InitializeView() \n"));

	_eye.SetPosition(eye);
	DirectX::XMStoreFloat3(&_lookat, lookat);
	DirectX::XMStoreFloat3(&_up, up);

	_view.SetView(eye, lookat, up);
}

void CameraBase::SetView()
{
	auto eye = _eye.GetPosition();
	auto lookat = DirectX::XMLoadFloat3(&_lookat);
	auto up = DirectX::XMLoadFloat3(&_up);

	_view.SetView(eye, lookat, up);
}

void CameraBase::Reset()
{
	DebugPrint(string("\t\t CameraBase::Reset() ...\n"));

	_eye.Reset();
	_view.Reset();
	_projection.Reset();
}