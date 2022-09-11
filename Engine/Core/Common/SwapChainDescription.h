
#pragma once

//
//	SwapChainDescription.h
//  Description of the swap chain object
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

namespace vxe {

	class SwapChainDescription {

	public:
		DXGI_SWAP_CHAIN_DESC1 _description;

	public:
		SwapChainDescription(float width = 0, float height = 0)
		{
			_description.Width = lround(width);
			_description.Height = lround(height);
			_description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			_description.Stereo = false;
			_description.SampleDesc.Count = 1;
			_description.SampleDesc.Quality = 0;
			_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			_description.BufferCount = 2;
			_description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			_description.Flags = 0;
			_description.Scaling = DXGI_SCALING_NONE;
			_description.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		}

		DXGI_SWAP_CHAIN_DESC1* Get() { return &_description; }

		void SetWidth(uint32_t width)  { _description.Width = width; }
		void SetHeight(uint32_t height) { _description.Height = height; }
		void SetFormat(DXGI_FORMAT format) { _description.Format = format; }
		void SetStereo(bool stereo) { _description.Stereo = stereo; }
		void SetSampleDescCount(uint32_t count) { _description.SampleDesc.Count = count; }
		void SetSampleDescQuality(uint32_t quality) { _description.SampleDesc.Quality = quality; }
		void SetBufferUsage(DXGI_USAGE usage) { _description.BufferUsage = usage; }
		void SetBufferCount(uint32_t count) { _description.BufferCount = count; }
		void SetSwapEffect(DXGI_SWAP_EFFECT effect) { _description.SwapEffect = effect; }
		void SetFlags(uint32_t flags) { _description.Flags = flags; }
		void SetScaling(DXGI_SCALING scaling) { _description.Scaling = scaling; }
		void SetAlphaMode(DXGI_ALPHA_MODE mode) { _description.AlphaMode = mode; }
	};

}