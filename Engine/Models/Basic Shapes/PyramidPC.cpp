
//
//	Pyramid.cpp
//  Defining cubes 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Pyramids.h"

using namespace vxe;
using namespace std;

using namespace DirectX;

Pyramid<VertexPositionColor, uint16_t>::Pyramid()
{
	DebugPrint(string("\t Pyramid<VertexPositionColor, uint16_t>::Ctor ...\n"));
}

 concurrency::task<void> Pyramid<VertexPositionColor, uint16_t>::CreateAsync(_In_ ID3D11Device2* device)
{
	DebugPrint(string("\t Pyramid<PositionColorVertex, uint16_t>::CreateAsync() ...\n"));

	vector<VertexPositionColor> vertices = {
		VertexPositionColor(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
		VertexPositionColor(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),
		VertexPositionColor(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
		VertexPositionColor(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)),
		VertexPositionColor(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f))
	};

	vector<uint16_t> indices = {
		0, 2, 1,
		1, 2, 3,
		0, 1, 4,
		1, 3, 4,
		3, 2, 4,
		2, 0, 4
	};

	return Mesh::CreateAsync(device, vertices, indices);
}

concurrency::task<void> Pyramid<VertexPositionColor, uint16_t>::LoadAsync(_In_ ID3D11Device2* device, const wstring&)
{
	DebugPrint(string("\t Pyramid<PositionNormalVertex, uint16_t>::LoadAsync() ...\n"));

	vector <VertexPositionColor> vertices;
	vector<uint16_t> indices;

	// Loading ...

	return Mesh::CreateAsync(device, vertices, indices);
}

concurrency::task<void> Pyramid<VertexPositionColor, uint16_t>::CreateAsync(_In_ ID3D11Device2* device, const vector<char>& memory)
{
	DebugPrint(string("\t Pyramid<VertexPositionColor, uint16_t>::CreateAsync() ...\n"));

	vector<VertexPositionColor> vertices{};
	vector<uint16_t> indices{};

	// Extract (parse) vertices from memory

	return Mesh::CreateAsync(device, vertices, indices);
}