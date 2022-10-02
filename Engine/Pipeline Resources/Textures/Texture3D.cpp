//
//	Texture3D.cpp
//  Encaspulating a 3D texture, a shader resource view and a sampler state
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Texture3D.h"
#include "..\..\Core\Common\DirectXHelper.h"

using namespace std;
using namespace vxe;

using namespace concurrency;

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::System;

using namespace DirectX;
using namespace DX;

task<void> Texture3D::CreateAsync(const wstring& filename)
{
	DebugPrint(wstring(L"\t Texture3D::CreateAsync()  ...\n" + filename + L"...\n"));

	return create_task(ReadDataAsync(filename)).then([this](const vector<uint8_t> data) {

		DebugPrint(string("\t -- A lambda: Creating a Texture3D \n"));

		D3D11_SUBRESOURCE_DATA srd = { 0 };
		srd.pSysMem = &data[0];
		srd.SysMemPitch = _description.Width;
		srd.SysMemSlicePitch = _description.Height * _description.Width;

		ThrowIfFailed(_device->CreateTexture3D(
			&_description,
			&srd,
			&_texture), __FILEW__, __LINE__);

		DebugPrint(std::string("\t -- A lambda: A Texture3D created. \n"));
	});
}

task<void> Texture3D::CreateAsyncFromMemory(const uint8_t* data)
{
	DebugPrint(wstring(L"\t Texture3D::CreateAsyncFromMemory() of ...\n" + to_wstring(\
		_description.Width * _description.Height * _description.Depth) + L" bytes in size ...\n"));

	return create_task([this, data]() {

		DebugPrint(string("\t -- A lambda: Creating a Texture3D \n"));

		D3D11_SUBRESOURCE_DATA srd = { 0 };
		srd.pSysMem = &data[0];
		srd.SysMemPitch = _description.Width;
		srd.SysMemSlicePitch = _description.Height * _description.Width;

		ThrowIfFailed(_device->CreateTexture3D(
			&_description,
			&srd,
			&_texture), __FILEW__, __LINE__);

		DebugPrint(std::string("\t -- A lambda: A Texture3D created. \n"));
	});
}
