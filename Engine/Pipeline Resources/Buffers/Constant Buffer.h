
#pragma once

//
//	Constant Buffer.h
//  Encapsulating a constant buffer (a structure which size is multiple of 16 bytes)
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Buffer Base.h"

#include "..\..\Pipeline Stages\Shaders\Shader Base.h"

namespace vxe {

	template <typename T>
	class ConstantBuffer : public BufferBase {

	public:
		using value_type = T;

		explicit ConstantBuffer(_In_ ID3D11Device2*,
			T* = nullptr,
			D3D11_USAGE = D3D11_USAGE_DEFAULT,
			uint32_t = 0,
			uint32_t = 0,
			uint32_t = 0);

		void Bind(_In_ ID3D11DeviceContext2* context,
			ProgrammableStage = ProgrammableStage::VertexShaderStage,
			uint32_t = 0,
			uint32_t = 1);

		void Update(_In_ ID3D11DeviceContext2* context, const T& data, 
			uint32_t subresource = 0, 
			const D3D11_BOX* box = nullptr, 
			uint32_t rawpitch = 0, 
			uint32_t depthpitch = 0)
		{
			//DebugPrint(std::string ("\t ConstantBuffer<") + typeid(T).name() + std::string(">::Update() ...\n"));

			context->UpdateSubresource(_buffer.Get(), subresource, box, &data, rawpitch, depthpitch);
		}

	private:
		ConstantBuffer(const ConstantBuffer&) = delete;
		ConstantBuffer(ConstantBuffer&&) = delete;

		ConstantBuffer& operator= (const ConstantBuffer&) = delete;
		ConstantBuffer& operator= (ConstantBuffer&&) = delete;
	};

#include "Constant Buffer.inl"

}