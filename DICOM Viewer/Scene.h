
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

#include "Textures.h"
#include "Animation.h"
#include "Camera.h"

namespace vxe {

	enum SceneObjectType {
		PointCloud, Volumetric, TriMesh, NumberOfObjects
	};

	class Scene {
	public:
		Scene();
		void LoadAssets(std::vector<concurrency::task<void>>&, std::shared_ptr<VanityCore>&);
		void SetCamera(std::shared_ptr<VanityCore>&);
		void SetTextures(std::shared_ptr<VanityCore>&);
		void Update(DX::StepTimer const&);
		void Render(std::shared_ptr<VanityCore>&);
		void Release();

		void DrawVolumetric(std::shared_ptr<VanityCore>&, bool=true);
		void DrawTriMesh(std::shared_ptr<VanityCore>&, bool=false);
		void DrawPointCloud(std::shared_ptr<VanityCore>&, bool=false);

	private:
		std::shared_ptr<SceneObject<DirectX::VertexPosition, uint16_t>> _pointcloud;
		std::shared_ptr<SceneObject<DirectX::VertexPosition, uint16_t>> _trisurface;
		std::shared_ptr<SceneObject<DirectX::VertexPositionTextureInstanced, uint16_t>> _volumetricslice;
		std::shared_ptr<SceneTexture<Texture2D>> _texArray;
		Animation _animation{};

		Camera _camera{};
	};

}