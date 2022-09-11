
#pragma once

//
//	Index Buffer.h
//  Encapsulating an index buffer object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Buffer Base.h"

namespace vxe {

	// uint32_t or unit16_t
	template <typename T>
	class IndexBuffer : public BufferBase {

	public:
		explicit IndexBuffer(_In_ ID3D11Device2*, void*, int32_t,
			D3D11_USAGE = D3D11_USAGE_DEFAULT,
			uint32_t = 0,
			uint32_t = 0,
			uint32_t = 0);

		void Bind(_In_ ID3D11DeviceContext2* context, DXGI_FORMAT format, uint32_t offset)
		{
			//DebugPrint(std::string("\t IndexBuffer<") + std::string(typeid(T).name()) + std::string(">::Bind() ...\n"));
			context->IASetIndexBuffer(_buffer.Get(), format, offset);
		}

	private:
		uint32_t _indexsize;

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&&) = delete;

		IndexBuffer& operator= (const IndexBuffer&) = delete;
		IndexBuffer& operator= (IndexBuffer&&) = delete;
	};

#include "Index Buffer.inl"

}
