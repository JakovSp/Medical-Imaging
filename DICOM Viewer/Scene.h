
#pragma once

//
//	Scene.h: 
//	Manages scene objects and the camera
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>
#include <Engine\Scene\Scene Object.h>

#include "Animation.h"

#include "Camera.h"

namespace vxe {

	class Scene {
		using VertexType_t = DirectX::VertexPosition;
		using IndexType_t = uint16_t;

	public:
		Scene();

		void LoadAssets(std::vector<concurrency::task<void>>&, std::shared_ptr<VanityCore>&);
		void SetCamera(std::shared_ptr<VanityCore>&);
		void Update(DX::StepTimer const&);
		void Render(std::shared_ptr<VanityCore>&);
		void Release();

	private:
		void Draw(std::shared_ptr<VanityCore>&, bool=true);

	private:
		std::shared_ptr<SceneObject<DirectX::VertexPosition, uint16_t>> _PCobject;
		std::shared_ptr<SceneObject<DirectX::VertexPosition, uint16_t>> _MCobject;
		Animation _animation{};

		Camera _camera{};
	};

}