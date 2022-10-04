#pragma once

#include "pch.h"

#include "Textures.h"
#include "ObjectAlignedSlices.h"
#include <Engine\Scene\Scene Object.h>
#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>

namespace vxe {
	class Volumetric : public SceneObject<DirectX::VertexPositionTextureInstanced, uint16_t> {
	public:
		Volumetric() : _orientation(Right) {}
		virtual void SetWorld(std::shared_ptr<WorldTransforms>& world) { _world = world; }
		virtual std::shared_ptr<WorldTransforms> GetWorld() { return _world; }

		virtual void SetMesh(std::shared_ptr<ObjectAlignedSlices> mesh) {
			_meshslices = mesh;
			_mesh = mesh;
		}

		void SwitchSamplingDirection(_In_ ID3D11Device2* device, DirectX::XMFLOAT4X4* view) {
			DirectX::XMMATRIX view_to_world = GetInverseMatrix(DirectX::XMLoadFloat4x4(view));
			DirectX::XMMATRIX world_to_model = GetInverseMatrix(DirectX::XMLoadFloat4x4(&GetWorld()->GetWorld()));
			DirectX::XMMATRIX view_to_model = view_to_world * world_to_model;
			DirectX::XMVECTOR v_direction_m = DirectX::XMVector4Transform(DirectX::FXMVECTOR{ 0, 0, 1, 0 }, view_to_model);
			DirectX::XMFLOAT4 direction;
			DirectX::XMStoreFloat4(&direction, v_direction_m);
			// DebugPrint(L"x: " + std::to_wstring(direction.x) + L", z: " + to_wstring(direction.z) + L" \n");


		
			if (abs(direction.z) > abs(direction.x)) {
				if (abs(direction.z) > abs(direction.y)) {
					_samplingmatrix = {
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 1.0f,
						0.0f, 0.0f, 0.0f, 0.0f
					};
					if (direction.z < 0) _orientation = Front;
					else _orientation = Back;
					
				 } else {
					_samplingmatrix = {
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f
					};
					if (direction.y < 0) _orientation = Top;
					else _orientation = Bottom;
				}
			 } else {
				if (abs(direction.x) > abs(direction.y)) {
					_samplingmatrix = {
						1.0f, 0.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f
					};
					if (direction.x < 0) _orientation = Right;
					else _orientation = Left;
				 } else {
					_samplingmatrix = {
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f
					};
					if (direction.y < 0) _orientation = Top;
					else _orientation = Bottom;
				}
			}

			_samplingbuffer = std::make_shared<ConstantBuffer<DirectX::XMFLOAT4X4>>(device, &_samplingmatrix);
		}

		virtual void Draw(_In_ ID3D11DeviceContext2* context, bool indexed=true) {
			_samplingbuffer->Update(context, _samplingmatrix);
			_samplingbuffer->Bind(context, ProgrammableStage::VertexShaderStage, _samplingbufferid);
			Bind(context);
			_meshslices->Draw(context, _orientation);
		}
		void SetTexture(std::shared_ptr<SceneTexture<Texture2D>> texturearray) {
			_textureslices = texturearray;
		}
		void BindTexture(std::shared_ptr<VanityCore>& vanitycore) {
			_textureslices->Bind(vanitycore);
		}

	private:
		DirectX::XMFLOAT4X4 _samplingmatrix;
		CubeFace _orientation;
		std::shared_ptr<ConstantBuffer<DirectX::XMFLOAT4X4>> _samplingbuffer;
		size_t _samplingbufferid = 3;
		std::shared_ptr<ObjectAlignedSlices> _meshslices;
		std::shared_ptr<SceneTexture<Texture2D>> _textureslices;
	};

}
