
#pragma once

//
//	Shader Base.h
//  A base class for shader objects
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

namespace vxe {

	enum class ProgrammableStage : uint8_t {
		VertexShaderStage,
		HullShaderStage,
		DomainShaderStage,
		GeometryShaderStage,
		PixelShaderStage,
		ComputeShaderStage,
		Size
	};

	class ShaderBase  {

	public:
		ShaderBase() { }

		virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2*, const std::wstring&) = 0;

		virtual void Bind(_In_ ID3D11DeviceContext2*, ID3D11ClassInstance* const*, uint32_t) = 0;

		virtual void Reset() = 0;

		virtual ~ShaderBase() = 0;
	};

	inline ShaderBase::~ShaderBase() {}

	inline std::string ToString(ProgrammableStage stage)
	{
		switch (stage) {

		default: return "What the #%@$";

		case ProgrammableStage::VertexShaderStage: return "VertexShaderStage";
		case ProgrammableStage::HullShaderStage: return "HullShaderStage";
		case ProgrammableStage::DomainShaderStage: return "DomainShaderStage";
		case ProgrammableStage::GeometryShaderStage: return "GeometryShaderStage";
		case ProgrammableStage::PixelShaderStage: return "PixelShaderStage";
		case ProgrammableStage::ComputeShaderStage: return "ComputeShaderStage";
		}
	}

}