
//
//	PyramidPC.cpp
//  Defining a pyramid with positions and colors
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Triangles.h"

using namespace vxe;
using namespace std;

using namespace DirectX;

Triangle<VertexPositionColor, uint16_t>::Triangle()
{
	DebugPrint(string("\t Triangle<VertexPositionColor, uint16_t>::Ctor ...\n"));
}

concurrency::task<void> Triangle<VertexPositionColor, uint16_t>::CreateAsync(_In_ ID3D11Device2* device)
{
	DebugPrint(string("\t Triangle<PositionColorVertex, uint16_t>::CreateAsync() ...\n"));

	vector<VertexPositionColor> vertices = {
		VertexPositionColor(XMFLOAT3(0.0f, 0.866f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
		VertexPositionColor(XMFLOAT3(1.0f, -0.866f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)),
		VertexPositionColor(XMFLOAT3(-1.0f, -0.866f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f))
	};

	vector<uint16_t> indices = { 0, 1, 2 };

	return Mesh::CreateAsync(device, vertices, indices);
}

concurrency::task<void> Triangle<VertexPositionColor, uint16_t>::LoadAsync(_In_ ID3D11Device2* device, const wstring&)
{
	DebugPrint(string("\t Triangle<PositionNormalVertex, uint16_t>::LoadAsync() ...\n"));

	vector <VertexPositionColor> vertices{};
	vector<uint16_t> indices{};

	// Loading ...

	return Mesh::CreateAsync(device, vertices, indices);
}

concurrency::task<void> Triangle<VertexPositionColor, uint16_t>::CreateAsync(_In_ ID3D11Device2* device, const vector<char>& data)
{
	DebugPrint(string("\t Triangle<VertexPositionColor, uint16_t>::CreateAsync() ...\n"));

	vector<VertexPositionColor> vertices{};
	vector<uint16_t> indices{};

	// Extract (parse) vertices from memory

	return Mesh::CreateAsync(device, vertices, indices);
}