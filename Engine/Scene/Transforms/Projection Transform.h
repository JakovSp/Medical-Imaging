
#pragma once

//
//	Projection Transform.h
//  Defining the projection transform
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "..\..\Utilities.h"
#include "..\..\Helpers.h"

#include "Data Transfer.h"
#include "View Transform.h"

namespace vxe {

	enum class Projection : uint8_t {
		Perspective,
		Orthographic,
		Size
	};

	inline std::string ToString(Projection p)
	{
		switch (p) {
			default: return "What the #%@$";

			case Projection::Perspective: return "Perspective";
			case Projection::Orthographic: return "Orthographic";
		}
	}

	class ProjectionTransform : public DataTransfer<DirectX::XMFLOAT4X4> {

	public:
		ProjectionTransform(_In_ ID3D11Device2*, Handedness = Handedness::LeftHanded, Projection = Projection::Perspective);

		void SetProjection(DirectX::CXMMATRIX, float, float, float, float);
		auto& GetProjection() { return _transfer; }

	private:
		Handedness _handedness;
		Projection _projectiontype;	// projection type
	};

}