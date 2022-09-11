
#pragma once

//
//	Pyramids.h
//  Defining pyramids 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\..\Utilities.h"
#include "..\Mesh.h"

namespace vxe {

	template <typename T, typename U> class Pyramid : public Mesh<T,U> {};

	template <>
	class Pyramid<DirectX::VertexPositionColor, uint16_t> : public Mesh<DirectX::VertexPositionColor, uint16_t> {

	public:
		Pyramid();

		// Vertices are hardcoded
		virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2*) override;

		// Loading from a file
		virtual concurrency::task<void>LoadAsync(_In_ ID3D11Device2*, const std::wstring&) override;

		// Creating from memory
		virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2*, const std::vector<char>&) override;
	};

}