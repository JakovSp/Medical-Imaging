//
//	Hull Shader.cpp
//  Encapsulating a pixel shader object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include "Hull Shader.h"

using namespace vxe;
using namespace std;
using namespace concurrency;

HullShader::HullShader() :
	_shader{ nullptr }
{}

task<void> HullShader::CreateAsync(_In_ ID3D11Device2* device, const wstring& filename) 
{
	DebugPrint(string("\t HullShader::CreateAsync() ...\n"));

	return create_task(DX::ReadDataAsync(filename)).then([this, device](const vector<uint8_t> data) {

		DebugPrint(string("\t -- A lambda: Creating a HS ...\n"));

		ThrowIfFailed(device->CreateHullShader(
			&data[0],
			data.size(),
			nullptr,
			&_shader), __FILEW__, __LINE__);

		DebugPrint(string("\t -- A lambda: a HS created.\n"));
	});
}

void HullShader::Bind(_In_ ID3D11DeviceContext2* context, ID3D11ClassInstance* const* instances, uint32_t count)
{
	//DebugPrint(string("\t HullShader::Bind() ...\n"));

	context->HSSetShader(_shader.Get(), instances, count);
}

void HullShader::Reset()
{
	DebugPrint(string("\t HullShader::Reset() ...\n"));

	_shader.Reset();
}
