
//
//	World Transforms.cpp
//  Defining the basic world transforms
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "World Transforms.h"

using namespace std;

using namespace vxe;
using namespace DirectX;

WorldTransforms::WorldTransforms(_In_ ID3D11Device2* device) :
	DataTransfer{ device }
{
	DebugPrint(string("\t WorldTransforms::Ctor ...\n"));

	auto identity = XMMatrixIdentity();

	XMStoreFloat4x4(&_transfer.world, identity);
	XMStoreFloat4x4(&_transfer.dlrow, identity);

	DebugPrint(string("\t\t World: ") + ToString(&_transfer.world) + string("\n"));
}

void WorldTransforms::Transform(CXMMATRIX scale, CXMMATRIX rotation, CXMMATRIX translation)
{
	DebugPrint(string("\t\t WorldTransforms::Transform() ...\n"));

	auto world = XMMatrixMultiply(XMMatrixMultiply(scale, rotation), translation);
	auto dlrow = GetInverseMatrix(world);

	XMStoreFloat4x4(&_transfer.world, XMMatrixTranspose(world));
	XMStoreFloat4x4(&_transfer.dlrow, dlrow);
}

void WorldTransforms::Translate(float x, float y, float z)
{
	auto world = XMMatrixTranslation(x, y, z);

	auto dlrow = GetInverseMatrix(world);

	XMStoreFloat4x4(&_transfer.world, XMMatrixTranspose(world));
	XMStoreFloat4x4(&_transfer.dlrow, dlrow);
}

void WorldTransforms::Scale(float x, float y, float z)
{
	auto world = XMMatrixScaling(x, y, z);

	auto dlrow = GetInverseMatrix(world);

	XMStoreFloat4x4(&_transfer.world, XMMatrixTranspose(world));
	XMStoreFloat4x4(&_transfer.dlrow, dlrow);
}

void WorldTransforms::RotateX(float radians)
{
	auto world = XMMatrixRotationX(radians);
	auto dlrow = GetInverseMatrix(world);

	XMStoreFloat4x4(&_transfer.world, XMMatrixTranspose(world));
	XMStoreFloat4x4(&_transfer.dlrow, dlrow);
}

void WorldTransforms::RotateY(float radians)
{
	auto world = XMMatrixRotationY(radians);
	auto dlrow = GetInverseMatrix(world);

	XMStoreFloat4x4(&_transfer.world, XMMatrixTranspose(world));
	XMStoreFloat4x4(&_transfer.dlrow, dlrow);
}

void WorldTransforms::RotateZ(float radians)
{
	auto world = XMMatrixRotationZ(radians);
	auto dlrow = GetInverseMatrix(world);

	XMStoreFloat4x4(&_transfer.world, XMMatrixTranspose(world));
	XMStoreFloat4x4(&_transfer.dlrow, dlrow);
}

void WorldTransforms::RotateYAndTranslate(float radians, float x, float y, float z)
{
	auto translation = XMMatrixTranslation(x, y, z);
	auto rotation = XMMatrixRotationY(radians);

	auto world = XMMatrixMultiply(rotation, translation);
	auto dlrow = GetInverseMatrix(world);

	XMStoreFloat4x4(&_transfer.world, XMMatrixTranspose(world));
	XMStoreFloat4x4(&_transfer.dlrow, dlrow);
}