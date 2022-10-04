#pragma once

#include "pch.h"

#include "Textures.h"
#include "ObjectAlignedSlices.h"
#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>

namespace vxe {
	class Volumetric : public SceneObject<DirectX::VertexPositionTextureInstanced, uint16_t> {
	public:
		Volumetric() : _orientation(Right) {}
		virtual void SetWorld(std::shared_ptr<WorldTransforms>& world);
		virtual std::shared_ptr<WorldTransforms> GetWorld();

		virtual void SetMesh(std::shared_ptr<ObjectAlignedSlices> mesh);
		void SwitchSamplingDirection(_In_ ID3D11Device2* device, DirectX::XMFLOAT4X4* view);

		virtual void Draw(_In_ ID3D11DeviceContext2* context, bool indexed = true);
		void SetTexture(std::shared_ptr<SceneTexture<Texture2D>> texturearray);
		void BindTexture(std::shared_ptr<VanityCore>& vanitycore);

	private:
		DirectX::XMFLOAT4X4 _samplingmatrix;
		CubeFace _orientation;
		std::shared_ptr<ConstantBuffer<DirectX::XMFLOAT4X4>> _samplingbuffer;
		size_t _samplingbufferid = 3;
		std::shared_ptr<ObjectAlignedSlices> _meshslices;
		std::shared_ptr<SceneTexture<Texture2D>> _textureslices;
	};

}
