
#pragma once

//
//	Directional Light.h
//  Implementing parallel lights
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\Utilities.h"
#include "..\Helpers.h"

#include "Light Base.h"
#include "..\Scene\Transforms\Data Transfer.h"

namespace vxe {

	using DirectionalLightTransfer = struct DirectionalLightTransfer : public LightBase  {
		DirectX::XMFLOAT3 direction;
		float Padding;
	};

	class DirectionalLight : public DataTransfer<DirectionalLightTransfer> {

	public:
		DirectionalLight(_In_ ID3D11Device2* device) : 
			DataTransfer{ device }
		{
			DebugPrint(std::string("\t DirectionalLight::Ctor ...\n"));
		}

		void SetComponents(const DirectX::XMFLOAT4& ambient, 
			const DirectX::XMFLOAT4& diffuse, 
			const DirectX::XMFLOAT4& specular,
			const DirectX::XMFLOAT3& direction)
		{
			DebugPrint(std::string("\t DirectionalLight::SetComponents() ...\n"));

			_transfer.ambient = ambient;
			_transfer.diffuse = diffuse;
			_transfer.specular = specular;
			_transfer.direction = direction;

			DebugPrint(std::string("\t\t Ambient: " + ToString(&_transfer.ambient) + "\n"));
			DebugPrint(std::string("\t\t Diffuse: " + ToString(&_transfer.diffuse) + "\n"));
			DebugPrint(std::string("\t\t Specular: " + ToString(&_transfer.specular) + "\n"));
			DebugPrint(std::string("\t\t Direction : " + ToString(&_transfer.direction) + "\n"));
		}

	};
}