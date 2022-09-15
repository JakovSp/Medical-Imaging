#pragma once

//
//	Geometry Shader.h
//  Encapsulating a geometry shader object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "..\..\Utilities.h" 

#include "Shader Base.h"
#include "..\..\Core\Common\DirectXHelper.h"

namespace vxe {

	class GeometryShader : public ShaderBase  {

	public:
		GeometryShader();

		concurrency::task<void> CreateAsync(_In_ ID3D11Device2*, const std::wstring&) override;
		virtual void Bind(_In_ ID3D11DeviceContext2*, ID3D11ClassInstance* const* = nullptr, uint32_t = 0) override;
		void Reset();

	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> _shader;

		GeometryShader(const GeometryShader&) = delete;
		GeometryShader(GeometryShader&&) = delete;

		GeometryShader& operator= (const GeometryShader&) = delete;
		GeometryShader& operator= (GeometryShader&&) = delete;
	};

}
