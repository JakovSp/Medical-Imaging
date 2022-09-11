
#pragma once

//
//	Buffer Base.h
//  A base class for DirectX buffers
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

namespace vxe {

	class BufferBase {

	public:
		BufferBase(uint32_t size) : 
			_buffer{ nullptr }, 
			_size{ size } 
		{}

		auto GetBuffer() { return _buffer.Get(); }

		void Reset()
		{
			DebugPrint(std::string("\t BufferBase::Reset() ...\n"));

			_buffer.Reset();
		}

		~BufferBase() {}

	protected:
		uint32_t _size;
		Microsoft::WRL::ComPtr<ID3D11Buffer> _buffer;
	};

}