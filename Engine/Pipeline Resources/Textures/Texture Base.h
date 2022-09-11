
#pragma once

//
//	Texture Base.h
//  Encaspulates views and a sampler state
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "..\..\Pipeline Stages\State Factory.h"
#include "..\Resource Factory.h"

#include "..\..\Pipeline Stages\Shaders\Shader Base.h"

namespace vxe {

	class TextureBase  {

	public:
		TextureBase(ID3D11Device2* device) : 
			_device{ device },
			_samplerstate{ nullptr },
			_srv{ nullptr },
			_uav{ nullptr } 
		{}

		bool CreateSamplerState(CD3D11_SAMPLER_DESC&);
		void CreateSamplerState();

		void BindSamplerState(_In_ ID3D11DeviceContext2*, uint32_t = 0, uint32_t = 1);

		virtual bool CreateShaderResourceView() = 0;
		virtual bool CreateUnorderedAccessView() = 0;

		void BindShaderResourceView(_In_ ID3D11DeviceContext2*,
			ProgrammableStage = ProgrammableStage::PixelShaderStage,
			uint32_t = 0,
			uint32_t = 1);

		void UnbindShaderResourceView(_In_ ID3D11DeviceContext2*,
			ProgrammableStage = ProgrammableStage::PixelShaderStage,
			uint32_t = 0,
			uint32_t = 1);

		void BindUnorderedAccessView(_In_ ID3D11DeviceContext2*,
			ProgrammableStage = ProgrammableStage::ComputeShaderStage,
			uint32_t = 0,
			uint32_t = 1);

		void UnbindUnorderedAccessView(_In_ ID3D11DeviceContext2*, uint32_t = 0, uint32_t = 1);

		void Reset();

	protected:
		Microsoft::WRL::ComPtr<ID3D11Device2> _device;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerstate;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> _uav;
		// Add additionals views

	private:
		TextureBase(const TextureBase&) = delete;
		TextureBase(TextureBase&&) = delete;

		TextureBase& operator= (const TextureBase&) = delete;
		TextureBase& operator= (TextureBase&&) = delete;
	};

}