
#pragma once

//
//	Camera.h: 
//	Manages the scene fixed camera 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include <Engine\Scene\Transforms\View Transform.h>
#include <Engine\Scene\Transforms\Projection Transform.h>

namespace vxe {

	class Camera {

	public:
		Camera();
		void Initialize(std::shared_ptr<VanityCore>&);
		void Bind(std::shared_ptr<VanityCore>&);
		void Release();

	private:
		DirectX::XMFLOAT3 _eye { 0.0f, 0.0f, 3.0f};
		DirectX::XMFLOAT3 _lookat{ 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 _up{ 0.0f, 1.0f, 0.0f };

		std::shared_ptr<ViewTransform> _view{};
		std::shared_ptr<ProjectionTransform> _projection{};

		uint32_t _viewregister = 1;
		uint32_t _projectionregister = 2;
	};

}
		