
#pragma once

//
//	Material.h
//  Encapsulates basic material properties
//
//  ? VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\Utilities.h"
#include "..\Helpers.h"

#include "Light Base.h"
#include "..\Scene\Transforms\Data Transfer.h"

namespace vxe {

	// Alpha is not important for lighting
	using MaterialTransfer = struct MaterialTransfer : public LightBase {
		//specular.w <-- spec power
		DirectX::XMFLOAT4 reflection;
	};

	class Material : public DataTransfer<MaterialTransfer> {

	public:
		Material(_In_ ID3D11Device2* device) : 
			DataTransfer{ device }
		{
			DebugPrint(std::string("\t Material::Ctor ...\n"));
		}

		void SetComponents(const DirectX::XMFLOAT4& ambient, 
			const DirectX::XMFLOAT4& diffuse, 
			const DirectX::XMFLOAT4& specular,
			const DirectX::XMFLOAT4& reflection)
		{
			DebugPrint(std::string("\t Material::SetComponents() ...\n"));

			_transfer.ambient = ambient;
			_transfer.diffuse = diffuse;
			_transfer.specular = specular;
			_transfer.reflection = reflection;

			DebugPrint(std::string("\t\t Ambient: " + ToString(&_transfer.ambient) + "\n"));
			DebugPrint(std::string("\t\t Diffuse: " + ToString(&_transfer.diffuse) + "\n"));
			DebugPrint(std::string("\t\t Specular: " + ToString(&_transfer.specular) + "\n"));
			DebugPrint(std::string("\t\t Reflection : " + ToString(&_transfer.reflection) + "\n"));
		}

	};
}