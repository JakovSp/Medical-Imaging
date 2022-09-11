
#pragma once

//
//	Light Base.h
//  Transfers a data (matrix, vector) via a constant buffer 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

namespace vxe {

	using LightBase = struct LightBase {
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;
	};

}