#pragma once

//
//	Hull Shader.h
//  Encapsulating a geometry shader object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "..\..\Utilities.h" 

#include "Shader Base.h"
#include "..\..\Core\Common\DirectXHelper.h"

namespace vxe {

	class HullShader : public ShaderBase  {

	public:
		HullShader();

		concurrency::task<void> CreateAsync(_In_ ID3D11Device2*, const std::wstring&) override;
		virtual void Bind(_In_ ID3D11DeviceContext2*, ID3D11ClassInstance* const* = nullptr, uint32_t = 0) override;
		void Reset();

	private:
		Microsoft::WRL::ComPtr<ID3D11HullShader> _shader;

		HullShader(const HullShader&) = delete;
		HullShader(HullShader&&) = delete;

		HullShader& operator= (const HullShader&) = delete;
		HullShader& operator= (HullShader&&) = delete;
	};

}
