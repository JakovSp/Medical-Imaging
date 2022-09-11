
#pragma once

//
//	Animation.h: 
//	Defining a rotation around Y using time and radians per second (RPS)
//
//  � VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

namespace vxe {

	class Animation {

	public:
		auto Angle(float time) { 
			_angle += time * _rps;
			return _angle;
		}

	private:
		float _rps = DirectX::XM_PI / 6.0f ;
		float _angle = 0.0f;
	};

}