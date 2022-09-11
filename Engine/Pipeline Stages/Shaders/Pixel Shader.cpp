
//
//	Pixel Shader.cpp
//  Encapsulating a pixel shader object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include "Pixel Shader.h"

using namespace vxe;
using namespace std;
using namespace concurrency;

PixelShader::PixelShader() :
	_shader{ nullptr }
{}

task<void> PixelShader::CreateAsync(_In_ ID3D11Device2* device, const wstring& filename) 
{
	DebugPrint(string("\t PixelShader::CreateAsync() ...\n"));

	return create_task(DX::ReadDataAsync(filename)).then([this, device](const vector<uint8_t> data) {

		DebugPrint(string("\t -- A lambda: Creating a PS ...\n"));

		ThrowIfFailed(device->CreatePixelShader(
			&data[0],
			data.size(),
			nullptr,
			&_shader), __FILEW__, __LINE__);

		DebugPrint(string("\t -- A lambda: a PS created.\n"));
	});
}

void PixelShader::Bind(_In_ ID3D11DeviceContext2* context, ID3D11ClassInstance* const* instances, uint32_t count)
{
	//DebugPrint(string("\t PixelShader::Bind() ...\n"));

	context->PSSetShader(_shader.Get(), instances, count);
}

void PixelShader::Reset()
{
	DebugPrint(string("\t PixelShader::Reset() ...\n"));

	_shader.Reset();
}
