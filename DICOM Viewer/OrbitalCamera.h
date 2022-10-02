
#pragma once

//
//	OrbitalCamera.h: 
//	Manages the scene fixed camera 
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include <Engine\Scene\Transforms\View Transform.h>
#include <Engine\Scene\Transforms\Projection Transform.h>

namespace vxe {

	class OrbitalCamera {

	public:
		OrbitalCamera();
		OrbitalCamera(float distance);

		void Initialize(std::shared_ptr<VanityCore>&);
		void Bind(std::shared_ptr<VanityCore>&);
		void Release();
		DirectX::XMFLOAT4X4 GetView() {
			return _view->GetView();
		}
		void Update(std::shared_ptr<VanityCore>& vc, const float&, const float&, const float&);

	private:
		float _distance{ 1.8f };
		float _alpha{ 0.0f };
		float _beta{ 0.0f };
		DirectX::XMFLOAT3 _eye{ _distance, 0.0f, 0.0f};
		DirectX::XMFLOAT3 _lookat{ 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 _up{ 0.0f, 1.0f, 0.0f };

		std::shared_ptr<ViewTransform> _view{};
		std::shared_ptr<ProjectionTransform> _projection{};

		uint32_t _viewregister = 1;
		uint32_t _projectionregister = 2;
	};

}
		