
//
//	Geometry Shader.cpp
//  Encapsulating a pixel shader object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include "Geometry Shader.h"

using namespace vxe;
using namespace std;
using namespace concurrency;

GeometryShader::GeometryShader() :
	_shader{ nullptr }
{}

task<void> GeometryShader::CreateAsync(_In_ ID3D11Device2* device, const wstring& filename) 
{
	DebugPrint(string("\t GeometryShader::CreateAsync() ...\n"));

	return create_task(DX::ReadDataAsync(filename)).then([this, device](const vector<uint8_t> data) {

		DebugPrint(string("\t -- A lambda: Creating a GS ...\n"));

		ThrowIfFailed(device->CreateGeometryShader(
			&data[0],
			data.size(),
			nullptr,
			&_shader), __FILEW__, __LINE__);

		DebugPrint(string("\t -- A lambda: a GS created.\n"));
	});
}

void GeometryShader::Bind(_In_ ID3D11DeviceContext2* context, ID3D11ClassInstance* const* instances, uint32_t count)
{
	//DebugPrint(string("\t GeometryShader::Bind() ...\n"));

	context->GSSetShader(_shader.Get(), instances, count);
}

void GeometryShader::Reset()
{
	DebugPrint(string("\t GeometryShader::Reset() ...\n"));

	_shader.Reset();
}
