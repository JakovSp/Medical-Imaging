#pragma once

//
//	Texture3D.h
//  Encaspulating a 3D texture 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Texture Base.h"

#include "..\..\Third Party\DirectX Tool Kit\DDS\DDSTextureLoader.h"

namespace vxe {
	class Texture3D : public TextureBase {

	protected:
		CD3D11_TEXTURE3D_DESC _description;
		Microsoft::WRL::ComPtr<ID3D11Texture3D> _texture;

	public:
		explicit Texture3D(ID3D11Device2* device,
			DXGI_FORMAT format,
			uint32_t width = 0,
			uint32_t height = 0,
			uint32_t depth = 1,
			uint32_t miplevels = 1,
			uint32_t bindflags = D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
			uint32_t cpuflags = 0,
			uint32_t samplecount = 1,
			uint32_t samplequality = 0,
			uint32_t miscflags = 0) :
			_texture{ nullptr },
			TextureBase(device),
			_description(format, width, height, depth, miplevels, bindflags, usage, cpuflags, miscflags)
		{
			DebugPrint(std::string("\t Texture3D::Ctor ...\n"));
		}

		concurrency::task<void> CreateAsync(const std::wstring&);
		concurrency::task<void> Texture3D::CreateAsyncFromMemory(const uint8_t* data);

		virtual bool CreateShaderResourceView() override
		{
			DebugPrint(std::string("\t Texture3D::CreateShaderResourceView() \n"));
			CD3D11_SHADER_RESOURCE_VIEW_DESC desc(D3D11_SRV_DIMENSION_TEXTURE3D, _description.Format, 0, _description.MipLevels);

			ResourceFactory::Instance().CreateShaderResourceView(_device.Get(), _texture.Get(), &desc, &_srv);

			return true;
		}

		virtual bool CreateUnorderedAccessView() override
		{
			DebugPrint(std::string("\t Texture3D::CreateUnorderedAccessView() \n"));

			uint32_t count = _description.Width * _description.Height;
			CD3D11_UNORDERED_ACCESS_VIEW_DESC desc(D3D11_UAV_DIMENSION_TEXTURE3D, _description.Format, 0, count);
			ResourceFactory::Instance().CreateUnorderedAccessView(_device.Get(), _texture.Get(), &desc, &_uav);

			return true;
		}

		auto GetTexture() const { return _texture.Get(); }
		uint32_t GetWidth() const { return _description.Width; }
		uint32_t GetHeight() const { return _description.Height; }
		uint32_t GetTexDepth() const { return _description.Depth; }

		void Reset()
		{
			DebugPrint(std::string("\t Texture3D::Reset() ...\n"));

			TextureBase::Reset();

			_texture.Reset();
		}

	public:
		Texture3D(const Texture3D&) = delete;
		Texture3D(Texture3D&&) = delete;

		Texture3D& operator= (const Texture3D&) = delete;
		Texture3D& operator= (Texture3D&&) = delete;
	};

}
