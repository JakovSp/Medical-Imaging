
#pragma once

//
//	Vertex Shader.h
//  Encapsulating a vertex shader object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include "Shader Base.h"

#include "..\..\Core\Common\DirectXHelper.h"

#include "..\..\Utilities.h"

namespace vxe {

	// for a vertex type
	template <typename T>
	class VertexShader : public ShaderBase  {

	public:
		VertexShader();

		concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device, const std::wstring& filename) override;
		virtual void Bind(_In_  ID3D11DeviceContext2*, ID3D11ClassInstance* const* = nullptr, uint32_t = 0) override;
		void Reset();

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> _shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> _layout;

		VertexShader(const VertexShader&) = delete;
		VertexShader(VertexShader&&) = delete;

		VertexShader& operator= (const VertexShader&) = delete;
		VertexShader& operator= (VertexShader&&) = delete;
	};

#include "Vertex Shader.inl"

}