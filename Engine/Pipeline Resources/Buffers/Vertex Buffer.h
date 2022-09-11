
#pragma once

//
//	Vertex Buffer.h
//  Encapsulating a vertex buffer object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Buffer Base.h"

namespace vxe {

	// a type of vertex
	template <typename T>
	class VertexBuffer : public BufferBase {

	public:
		using value_type = T;

		explicit VertexBuffer(_In_ ID3D11Device2*, const T*, int32_t,
			D3D11_USAGE = D3D11_USAGE_DEFAULT,
			uint32_t = 0,
			uint32_t = 0,
			uint32_t = 0);

		void Bind(_In_ ID3D11DeviceContext2* context, uint32_t slot, uint32_t count, const uint32_t offset)
		{
			//DebugPrint(std::string("\t VertexBuffer<") + typeid(T).name() + std::string(">::Bind() ...\n"));
			context->IASetVertexBuffers(slot, count, _buffer.GetAddressOf(), &_vertexsize, &offset);
		}

	private:
		uint32_t _vertexsize;

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) = delete;

		VertexBuffer& operator= (const VertexBuffer&) = delete;
		VertexBuffer& operator= (VertexBuffer&&) = delete;
	};

#include "Vertex Buffer.inl"

}
