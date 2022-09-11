
#pragma once

//
//	View Transform.h
//  Defining the view transform
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "..\..\Utilities.h"
#include "..\..\Helpers.h"

#include "Data Transfer.h"

namespace vxe {

	enum class Handedness : uint8_t {
		LeftHanded,
		RightHanded,
		Size
	};

	inline std::string ToString(Handedness h)
	{
		switch (h) {

		default: return "What the #%@$";

		case Handedness::LeftHanded: return "LeftHanded";
		case Handedness::RightHanded: return "RightHanded";
		}
	}

	class ViewTransform : public DataTransfer<DirectX::XMFLOAT4X4> {

	public:
		ViewTransform(_In_ ID3D11Device2*, Handedness = Handedness::LeftHanded);

		void SetView(DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR);

		auto& GetView() const { return _transfer; }

	private:
		Handedness _handedness;
	};

}