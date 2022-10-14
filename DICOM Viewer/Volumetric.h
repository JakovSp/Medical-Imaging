#pragma once

#include "pch.h"

#include "Textures.h"
#include "ObjectAlignedSlices.h"
#include "BoxSlice.h"
#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>

namespace vxe {
	class OASBVolume : public SceneObject<DirectX::VertexPositionTextureInstanced, uint16_t> {
	public:
		OASBVolume() : _orientation(Right) {}
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

	// TODO: check if data needs to be serialized with pragma pack
#pragma pack(push, 4)
	struct VASPerFrameData {
		DirectX::XMFLOAT4 vecVertices[8];
		DirectX::XMFLOAT4 vecView;
		float samplingrate;
		uint32_t frontIndex;
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct VASConstantData {
		uint32_t nSequence[64] = {
			0, 1, 2, 3, 4, 5, 6, 7,
			1, 4, 5, 0, 3, 7, 2, 6,
			2, 6, 0, 5, 7, 3, 1, 4,
			3, 0, 6, 4, 1, 2, 7, 5,
			4, 3, 7, 1, 0, 6, 5, 2,
			5, 2, 1, 7, 6, 0, 4, 3, 
			6, 7, 3, 2, 5, 4, 0, 1,
			7, 5, 4, 6, 2, 1, 3, 0
		};

		uint32_t v1[24] = {
			0, 1, 4, 1,
			1, 0, 1, 4,
			0, 2, 5, 2,
			2, 0, 2, 5,
			0, 3, 6, 3,
			3, 0, 3, 6,
		};

		uint32_t v2[24] = {
			1, 4, 7, 5,
			5, 1, 4, 7,
			2, 5, 7, 6,
			6, 2, 5, 7,
			3, 6, 7, 4,
			4, 3, 6, 7,
		};
	};
#pragma pack(pop)

	class VASBVolume : public SceneObject<DirectX::VertexIndex, uint16_t> {
	public:
		VASBVolume(_In_ ID3D11Device2* device, int32_t samples_x, uint32_t samples_y, uint32_t samples_z, float dx, float dy, float dz);
		void SwitchSamplingDirection(_In_ ID3D11Device2* device, DirectX::XMFLOAT4X4* view);
		virtual void SetMesh(std::shared_ptr<BoxSlice> mesh);
		void SetTexture(std::shared_ptr<SceneTexture<Texture3D>> texturearray);
		void BindTexture(std::shared_ptr<VanityCore>& vanitycore);
		virtual void Draw(_In_ ID3D11DeviceContext2* context, bool indexed = true);
	private:
		VASConstantData _constantdata;
		VASPerFrameData _perframedata;
		std::shared_ptr<ConstantBuffer<VASPerFrameData>> _perframebuffer;
		std::shared_ptr<ConstantBuffer<VASConstantData>> _constantbuffer;
		std::shared_ptr<BoxSlice> _slicepolygon;
		std::shared_ptr<SceneTexture<Texture3D>> _texture3D;
	};
}
