
//
//	Projection Transform.cpp
//  Defining the projection transform
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Projection Transform.h"

using namespace std;

using namespace vxe;
using namespace DirectX;

ProjectionTransform::ProjectionTransform(_In_ ID3D11Device2* device, Handedness h, Projection t) :
	DataTransfer{ device },
	_handedness{ h },
	_projectiontype{ t }
{
	DebugPrint(string("\t ProjectionTransform::Ctor ...\n"));
	DebugPrint(string("\t\t Handedness: ") + ToString(_handedness) + string("\n"));
	DebugPrint(string("\t\t Projection: ") + ToString(_projectiontype) + string("\n"));
}

void ProjectionTransform::SetProjection(DirectX::CXMMATRIX orientation, float a, float b, float zn, float zf)
{
	DebugPrint(string("\t ProjectionTransform::SetProjection() ... \n"));

	XMMATRIX projection;

	if (_handedness == Handedness::LeftHanded) {

		if (_projectiontype == Projection::Perspective)
			projection = XMMatrixPerspectiveFovLH(a, b, zn, zf);
		else
			projection = XMMatrixOrthographicLH(a, b, zn, zf);
	} else {

		if (_projectiontype == Projection::Perspective)
			projection = XMMatrixPerspectiveFovRH(a, b, zn, zf);
		else
			projection = XMMatrixOrthographicRH(a, b, zn, zf);
	}

#if defined (_DEBUG)
	XMFLOAT4X4 P;
	XMStoreFloat4x4(&P, projection);
	DebugPrint(string("\t\t Projection: " + ToString(&P) + "\n"));
#endif

	XMStoreFloat4x4(&_transfer, XMMatrixTranspose(projection * orientation));
}