
#pragma once

//
//	World Transforms.h
//  Defining the basic world transforms
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\..\Utilities.h"
#include "..\..\Helpers.h"

#include "Data Transfer.h"

namespace vxe {

	using Transfer = struct Transfer {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 dlrow;
	};

	class WorldTransforms : public DataTransfer<Transfer> {

	public:
		WorldTransforms(_In_ ID3D11Device2*);

		void SetWorld(DirectX::XMFLOAT4X4& world) { _transfer.world = world; }
		DirectX::XMFLOAT4X4& GetWorld() { return _transfer.world; }

		void SetDlrow(DirectX::XMFLOAT4X4& dlrow) { _transfer.dlrow = dlrow; }
		DirectX::XMFLOAT4X4& GetDlrow() { return _transfer.dlrow; }

		void Transform(DirectX::CXMMATRIX, DirectX::CXMMATRIX, DirectX::CXMMATRIX);

		void Translate(float, float, float);
		void Scale(float, float, float);

		void RotateX(float);
		void RotateY(float);
		void RotateZ(float);

		void RotateYAndTranslate(float, float=0, float=0, float=0);
	};

}