
//
//	Texture Base.cpp
//  Encaspulates views and a sampler state
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "texture Base.h"

using namespace std;
using namespace vxe;

bool TextureBase::CreateSamplerState(CD3D11_SAMPLER_DESC& desc)
{
	DebugPrint(string("\t TextureBase::CreateSamplerState() ...\n"));

	StateFactory::Instance().CreateSamplerState(
		_device.Get(),
		&desc,
		&_samplerstate);

	return true;
}

void TextureBase::CreateSamplerState()
{
	DebugPrint(string("\t TextureBase::CreateSamplerState() ...\n"));

	CD3D11_SAMPLER_DESC desc(CD3D11_DEFAULT{});

	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	//desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	// Border address mode is not used, so this parameter is ignored.
	for (int i = 0; i < 4; ++i) desc.BorderColor[i] = 0.0f;

	StateFactory::Instance().CreateSamplerState(
		_device.Get(),
		&desc,
		&_samplerstate);
}

void TextureBase::BindSamplerState(_In_ ID3D11DeviceContext2* context, uint32_t slot, uint32_t count)
{
	DebugPrint(string("\t TextureBase::BindSamplerState() ...\n"));

	if (_samplerstate == nullptr) {
		DebugPrint(string("\t\t _samplerstate is nullptr. \n"));

		return;
	}

	context->PSSetSamplers(slot, count, _samplerstate.GetAddressOf());
}

void TextureBase::BindShaderResourceView(_In_ ID3D11DeviceContext2* context, ProgrammableStage shader, uint32_t slot , uint32_t count)
{
	DebugPrint(string("\t TextureBase::BindShaderResourceView() ...\n"));

	if (_srv == nullptr) {
		DebugPrint(string("\t\t _srv is nullptr. \n"));

		return;
	}

	if (shader == ProgrammableStage::VertexShaderStage) 
		context->VSSetShaderResources(slot, count, _srv.GetAddressOf());
	else if (shader == ProgrammableStage::PixelShaderStage) 
		context->PSSetShaderResources(slot, count, _srv.GetAddressOf());
	else if (shader == ProgrammableStage::ComputeShaderStage) 
		context->CSSetShaderResources(slot, count, _srv.GetAddressOf());
	else
		return;
}

void TextureBase::UnbindShaderResourceView(_In_ ID3D11DeviceContext2* context, ProgrammableStage shader, uint32_t slot, uint32_t count)
{
	DebugPrint(string("\t TextureBase::UnbindShaderResourceView() ...\n"));

	ID3D11ShaderResourceView* srv = nullptr;

	if (shader == ProgrammableStage::VertexShaderStage) 
		context->VSSetShaderResources(slot, count, &srv);
	else if (shader == ProgrammableStage::PixelShaderStage) 
		context->PSSetShaderResources(slot, count, &srv);
	else if (shader == ProgrammableStage::ComputeShaderStage) 
		context->CSSetShaderResources(slot, count, &srv);
	else
		return;
}

void TextureBase::BindUnorderedAccessView(_In_ ID3D11DeviceContext2* context, ProgrammableStage, uint32_t slot, uint32_t count)
{
	DebugPrint(string("\t TextureBase::BindUnorderedAccessView() ...\n"));

	if (_uav == nullptr) {
		DebugPrint(string("\t\t _srv is nullptr. \n"));

		return;
	}

	context->CSSetUnorderedAccessViews(slot, count, _uav.GetAddressOf(), nullptr);
}

void TextureBase::UnbindUnorderedAccessView(_In_ ID3D11DeviceContext2* context,
	uint32_t slot,
	uint32_t count)
{
	DebugPrint(string("\t TextureBase::UnbindUnorderedAccessView() ...\n"));

	// TO DO: assert _uav
	ID3D11UnorderedAccessView* uav = nullptr;

	context->CSSetUnorderedAccessViews(slot, count, &uav, nullptr);
}

void TextureBase::Reset()
{
	DebugPrint(string("\t TextureBase::Reset() ...\n"));

	if (_samplerstate != nullptr) _samplerstate.Reset();

	if (_srv != nullptr) _srv.Reset();
	if (_uav != nullptr)_uav.Reset();
}