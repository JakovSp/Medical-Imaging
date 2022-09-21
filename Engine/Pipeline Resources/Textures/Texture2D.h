
#pragma once

//
//	Texture2D.h
//  Encaspulating a 2D texture 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Texture Base.h"

#include "..\..\Third Party\DirectX Tool Kit\DDS\DDSTextureLoader.h"

namespace vxe {

	class Texture2D : public TextureBase {

	protected:
		CD3D11_TEXTURE2D_DESC _description;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture;

	public:
		explicit Texture2D(ID3D11Device2* device,
			DXGI_FORMAT format,
			uint32_t width = 0,
			uint32_t height = 0,
			uint32_t arraysize = 1,
			uint32_t miplevels = 1,
			uint32_t bindflags = D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
			uint32_t cpuflags = 0,
			uint32_t samplecount = 1,
			uint32_t samplequality = 0,
			uint32_t miscflags = 0) :
			_texture{ nullptr },
			TextureBase(device),
			_description(format, width, height, arraysize, miplevels, bindflags, usage, cpuflags, samplecount, samplequality, miscflags)
		{
			DebugPrint(std::string("\t Texture2D::Ctor ...\n"));
		}

		concurrency::task<void> CreateAsyncFromMemory(const uint8_t* data, uint32_t = 1024, uint32_t slicepitch = 0);
		concurrency::task<void> CreateAsync(const std::wstring&, uint32_t = 1024, uint32_t = 0);

		// The wrappers for DirectX Tool Kit DDS functions
		void CreateDDS(ID3D11DeviceContext*, _In_z_ const std::wstring&, _In_ size_t = 0, _Out_opt_ DirectX::DDS_ALPHA_MODE* = nullptr);

		concurrency::task<void> CreateDDSAsync(_In_opt_ ID3D11DeviceContext*, _In_z_ const std::wstring&, _In_ size_t = 0, _Out_opt_ DirectX::DDS_ALPHA_MODE* = nullptr);

		virtual bool CreateShaderResourceView() override
		{
			DebugPrint(std::string("\t Texture2D::CreateShaderResourceView() \n"));
			CD3D11_SHADER_RESOURCE_VIEW_DESC desc(D3D11_SRV_DIMENSION_TEXTURE2D, _description.Format, 0, _description.MipLevels);

			ResourceFactory::Instance().CreateShaderResourceView(_device.Get(), _texture.Get(), &desc, &_srv);

			return true;
		}

		virtual bool CreateUnorderedAccessView() override
		{
			DebugPrint(std::string("\t Texture2D::CreateUnorderedAccessView() \n"));

			uint32_t count = _description.Width * _description.Height;
			CD3D11_UNORDERED_ACCESS_VIEW_DESC desc(D3D11_UAV_DIMENSION_TEXTURE2D, _description.Format, 0, count);
			ResourceFactory::Instance().CreateUnorderedAccessView(_device.Get(), _texture.Get(), &desc, &_uav);

			return true;
		}

		auto GetTexture() const { return _texture.Get(); }

		uint32_t GetWidth() const { return _description.Width; }

		uint32_t GetHeight() const { return _description.Height; }

		void Reset()
		{
			DebugPrint(std::string("\t Texture2D::Reset() ...\n"));

			TextureBase::Reset();

			_texture.Reset();
		}

	public:
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;

		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (Texture2D&&) = delete;
	};

}