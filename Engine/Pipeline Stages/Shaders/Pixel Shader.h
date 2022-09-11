
#pragma once

//
//	Pixel Shader.h
//  Encapsulating a pixel shader object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "..\..\Utilities.h" 

#include "Shader Base.h"
#include "..\..\Core\Common\DirectXHelper.h"

namespace vxe {

	class PixelShader : public ShaderBase  {

	public:
		PixelShader();

		concurrency::task<void> CreateAsync(_In_ ID3D11Device2*, const std::wstring&) override;
		virtual void Bind(_In_ ID3D11DeviceContext2*, ID3D11ClassInstance* const* = nullptr, uint32_t = 0) override;
		void Reset();

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> _shader;

		PixelShader(const PixelShader&) = delete;
		PixelShader(PixelShader&&) = delete;

		PixelShader& operator= (const PixelShader&) = delete;
		PixelShader& operator= (PixelShader&&) = delete;
	};

}