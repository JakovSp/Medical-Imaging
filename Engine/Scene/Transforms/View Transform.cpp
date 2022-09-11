
//
//	View Transform.cpp
//  Defining the view transform
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "View Transform.h"

using namespace std;

using namespace vxe;
using namespace DirectX;

ViewTransform::ViewTransform(_In_ ID3D11Device2* device, Handedness handedness) :
	DataTransfer{ device },
	_handedness{ handedness }
{
	DebugPrint(string("\t ViewTransform::Ctor ...\n"));
	DebugPrint(string("\t\t Handedness: ") + ToString(_handedness) + string("\n"));
}

void ViewTransform::SetView(FXMVECTOR eye, FXMVECTOR at,FXMVECTOR up)
{
	__Once(DebugPrint(string("\t ViewTransform::SetView() ... \n")));

	XMMATRIX view;

	if (_handedness == Handedness::LeftHanded)
		view = XMMatrixLookAtLH(eye, at, up);
	else
		view = XMMatrixLookAtRH(eye, at, up);

	XMStoreFloat4x4(&_transfer, XMMatrixTranspose(view));

#if defined (_DEBUG)
	XMFLOAT4X4 V;
	XMStoreFloat4x4(&V, view);
	__Once(DebugPrint(string("\t\t View: " + ToString(&V) + "\n")));
#endif

}