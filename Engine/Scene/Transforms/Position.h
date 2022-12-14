
#pragma once

//
//	Position.h
//  Defining the a position to be transfered to the GPU
//
//  ? VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\..\Utilities.h"
#include "..\..\Helpers.h"

#include "Data Transfer.h"

namespace vxe {

	class Position : public DataTransfer<DirectX::XMFLOAT4> {

	public:
		Position(_In_ ID3D11Device2* device) : 
			DataTransfer{ device }
		{
			DebugPrint(std::string("\t Position::Ctor ...\n"));
		}

		auto GetPosition() const { return DirectX::XMLoadFloat4(&_transfer); }
		void SetPosition(DirectX::FXMVECTOR position)
		{
			//DebugPrint(std::string("\t Position::SetPosition() ... \n"));

			DirectX::XMStoreFloat4(&_transfer, position);

#if defined (_DEBUG)
			DirectX::XMFLOAT4 p;
			DirectX::XMStoreFloat4(&p, position);
			//DebugPrint(std::string("\t\t Eye: " + ToString(&p) + "\n"));
#endif
		}

		void ChangePosition(float dx, float dy, float dz)
		{
			_transfer.x += dx;
			_transfer.y += dy;
			_transfer.z += dz;
		}

	};

}