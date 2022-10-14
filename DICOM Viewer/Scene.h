
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
#include <Engine\Scene\Input\Input Controller.h>

#include "Textures.h"
#include "Animation.h"
#include "OrbitalCamera.h"

#include "Volumetric.h"
#include "Box.h"

namespace vxe {

	enum SceneObjectType {
		VolumetricVAMesh, Wireframe, NumberOfObjectTypes
	};

	class Scene {
	public:
		Scene();
		void LoadAssets(std::vector<concurrency::task<void>>&, std::shared_ptr<VanityCore>&);
		void SetCamera(std::shared_ptr<VanityCore>&);
		void SetTextures(std::shared_ptr<VanityCore>&);
		void Update(DX::StepTimer const&, std::shared_ptr<VanityCore>& vanitycore, InputController^ ic);
		// void Render(std::shared_ptr<VanityCore>&);
		void Release();

		void DrawVAVolumetric(std::shared_ptr<VanityCore>&, bool=true);
		void DrawOAVolumetric(std::shared_ptr<VanityCore>&, bool=true);
		void DrawTriMesh(std::shared_ptr<VanityCore>&, bool=false);
		void DrawPointCloud(std::shared_ptr<VanityCore>&, bool=false);

	private:
		//std::shared_ptr<SceneObject<DirectX::VertexPosition, uint16_t>> _pointcloud;
		//std::shared_ptr<SceneObject<DirectX::VertexPosition, uint16_t>> _trisurface;
		std::shared_ptr<SceneObject<DirectX::VertexPosition, uint16_t>> _box;
		std::shared_ptr<OASBVolume> _volumetric;
		std::shared_ptr<VASBVolume> _VAvolumetric;
		std::shared_ptr<SceneTexture<Texture2D>> _texArray;
		std::shared_ptr<SceneTexture<Texture3D>> _tex3D;
		Animation _animation{};
		OrbitalCamera _camera{};

		DirectX::XMFLOAT2 _pointer{0.0f, 0.0f};
	};
}