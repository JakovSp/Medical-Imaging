

#pragma once

//
//	Triangles.h
//  Defining triangles 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\..\Utilities.h"
#include "..\Mesh.h"

namespace vxe {

	template <typename T, typename U> class Triangle : public Mesh<T,U> {};

	template <>
	class Triangle<DirectX::VertexPositionColor, uint16_t> : public Mesh<DirectX::VertexPositionColor, uint16_t> {

	public:
		Triangle();

		// Vertices are hardcoded
		virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2*) override;

		// Loading from a file
		virtual concurrency::task<void> LoadAsync(_In_ ID3D11Device2*, const std::wstring&) override;

		// Creating from memory
		virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device, const std::vector<char>& data) override;
	};

}