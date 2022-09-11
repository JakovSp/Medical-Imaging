
//
//	CubePN.cpp
//  Defining a cube with a position and normal 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "Cubes.h"

using namespace vxe;
using namespace std;

using namespace DirectX;

Cube<VertexPositionNormal, uint16_t>::Cube()
{
	DebugPrint(string("\t Cube<VertexPositionNormal, uint16_t>::Ctor ...\n"));
}

concurrency::task<void> Cube<VertexPositionNormal, uint16_t>::CreateAsync(ID3D11Device2* device)
{
	DebugPrint(string("\t Cube<VertexPositionNormal, uint16_t>::CreateAsync() ...\n"));

	vector<VertexPositionNormal> vertices = {

		// Side +z
		VertexPositionNormal(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),		
		VertexPositionNormal(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),
		VertexPositionNormal(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),
		VertexPositionNormal(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)),

		// Side -z
		VertexPositionNormal(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)),    
		VertexPositionNormal(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)),
		VertexPositionNormal(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)),
		VertexPositionNormal(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)),

		// Side +y
		VertexPositionNormal(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)),		
		VertexPositionNormal(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)),
		VertexPositionNormal(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)),
		VertexPositionNormal(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)),

		// Side -y
		VertexPositionNormal(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)),	
		VertexPositionNormal(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)),
		VertexPositionNormal(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)),
		VertexPositionNormal(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)),

		// Side +x
		VertexPositionNormal(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),		
		VertexPositionNormal(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),
		VertexPositionNormal(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),
		VertexPositionNormal(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)),

		// Side -x
		VertexPositionNormal(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)),	
		VertexPositionNormal(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		VertexPositionNormal(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		VertexPositionNormal(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)),
	};

	vector<uint16_t> indices = {
		0, 1, 2,		// side 1
		2, 1, 3,
		4, 5, 6,		// side 2
		6, 5, 7,
		8, 9, 10,		// side 3
		10, 9, 11,
		12, 13, 14,		// side 4
		14, 13, 15,
		16, 17, 18,		// side 5
		18, 17, 19,
		20, 21, 22,		// side 6
		22, 21, 23,
	};

	return Mesh::CreateAsync(device, vertices, indices);
}

concurrency::task<void> Cube<VertexPositionNormal, uint16_t>::LoadAsync(_In_ ID3D11Device2* device, const wstring&)
{
	DebugPrint(string("\t Cube<VertexPositionNormal, uint16_t>::LaodAsync() ...\n"));

	vector<VertexPositionNormal> vertices;
	vector<uint16_t> indices;

	// Loading from a file

	return Mesh::CreateAsync(device, vertices, indices);
}

concurrency::task<void> Cube<VertexPositionNormal, uint16_t>::CreateAsync(_In_ ID3D11Device2* device, const vector<char>&)
{
	DebugPrint(string("\t Cube<VertexPositionNormal, uint16_t>::CreateAsync() ...\n"));

	vector<VertexPositionNormal> vertices;
	vector<uint16_t> indices;

	// Extract (parse) vertices from memory

	return Mesh::CreateAsync(device, vertices, indices);
}