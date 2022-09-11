
#pragma once

//
//	Utilities.h
//  Various utilities 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

namespace vxe {

	template <typename U, typename T>
	U violent_cast(T source) {

		union {
			U destination;
			T source;
		} convertor;

		convertor.source = source;
		return convertor.destination;
	}

	template <typename U, typename T>
	U violent_covert_to(T source) {
		return T::unimplemented_conversion;
	}

#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }

#define SAFE_UNLOAD(p) { if(p) { p->Unload(); p = nullptr; } }

#if defined (_DEBUG)

	inline void DebugPrint(std::string& text)
	{
		OutputDebugStringA(text.c_str());
	}

	inline void DebugPrint(std::wstring& text)
	{
		OutputDebugStringW(text.c_str());
	}

#define __Once(f)												\
	{															\
		static std::atomic<bool> __vxeonce(false);				\
		if (!__vxeonce) f;										\
		__vxeonce.exchange(true);								\
	}

#else
#define DebugPrint(x);
#define __Once(x);
#endif

	inline void ThrowIfFailed(HRESULT hr, const wchar_t *file, unsigned long line)
	{
		if (FAILED(hr)) {

			std::wstringstream wss;

			wss << L"Failed HRESULT 0x" << std::hex << std::uppercase << static_cast<unsigned long>(hr)
				<< L" in file " << (file == nullptr ? L"(No filename passed)" : file)
				<< L" at line " << std::dec << line << L"." << std::endl;

#if defined (_DEBUG)
			DebugPrint(std::wstring(wss.str().c_str()));
			throw Platform::Exception::CreateException(hr, ref new Platform::String(wss.str().c_str()));
#endif
			throw Platform::Exception::CreateException(hr, ref new Platform::String(wss.str().c_str()));
		}
	}

}