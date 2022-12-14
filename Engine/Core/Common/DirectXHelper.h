
#pragma once

//
//	DirectXHelper.h
//  Various Helpers
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

namespace DX
{
	// Function that reads from a binary file asynchronously.
	inline Concurrency::task<std::vector<uint8_t>> ReadDataAsync(const std::wstring& filename)
	{
		using namespace Windows::Storage;
		using namespace Concurrency;

		// auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;

		return create_task(StorageFile::GetFileFromPathAsync(Platform::StringReference(filename.c_str()))).then([] (StorageFile^ file) 
		{
			return FileIO::ReadBufferAsync(file);
		}).then([] (Streams::IBuffer^ fileBuffer) -> std::vector<uint8_t>
		{
			std::vector<uint8_t> returnBuffer;
			returnBuffer.resize(fileBuffer->Length);
			Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(Platform::ArrayReference<uint8_t>(returnBuffer.data(), fileBuffer->Length));
			return returnBuffer;
		});
	}

	// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const float dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
	}

#if defined(_DEBUG)
	// Check for SDK Layer support.
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
			nullptr,                    // Any feature level will do.
			0,
			D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
			nullptr,                    // No need to keep the D3D device reference.
			nullptr,                    // No need to know the feature level.
			nullptr                     // No need to keep the D3D device context reference.
			);

		return SUCCEEDED(hr);
	}
#endif
}

// Taken from DirectX Tool Kit June 2014
namespace DirectX
{

	// Helper sets a D3D resource name string (used by PIX and debug layer leak reporting).
	template<UINT TNameLength>
	inline void SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_z_ const char(&name)[TNameLength])
	{
#if !defined(NO_D3D11_DEBUG_NAME) && ( defined(_DEBUG) || defined(PROFILE) )
		resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
		UNREFERENCED_PARAMETER(resource);
		UNREFERENCED_PARAMETER(name);
#endif
	}

}

