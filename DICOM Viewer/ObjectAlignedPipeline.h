#pragma once

#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>

#include <Engine\Pipeline Stages\Shaders\Vertex Shader.h>
#include <Engine\Pipeline Stages\Shaders\Pixel Shader.h>


namespace vxe {
	class ObjectAlignedPipeline {
	public:
		ObjectAlignedPipeline() {
			DebugPrint(std::string("\t Pipeline::Ctor... \n"));
		}

		void LoadShaders(std::vector<concurrency::task<void>>& tasks, std::shared_ptr<VanityCore>& vanitycore) {
			DebugPrint(std::string("\t Pipeline::LoadShaders() ... \n"));

			auto device = vanitycore->GetD3DDevice();
			
			_vshader = std::make_shared<VertexShader<DirectX::VertexPositionTextureInstanced>>();
			tasks.push_back(_vshader->CreateAsync(device, L"VolumetricVS.cso"));
			_vshaderside = std::make_shared<VertexShader<DirectX::VertexPositionTextureInstanced>>();
			tasks.push_back(_vshader->CreateAsync(device, L"VolumetricVSside.cso"));
			_pixelshader = std::make_shared<PixelShader>();
			tasks.push_back(_pixelshader->CreateAsync(device, L"VolumetricPS.cso"));
		}

		void BindShaders(std::shared_ptr<VanityCore>& vanitycore) {
			auto context = vanitycore->GetD3DDeviceContext();

			_vshader->Bind(context);
			_pixelshader->Bind(context);
		}

		void SwitchShaders(std::shared_ptr<VanityCore>& vanitycore, DirectX::XMFLOAT4 const& direction) {
			auto context = vanitycore->GetD3DDeviceContext();
			if (abs(direction.x) > abs(direction.z)) {
				if (!front) {
					_vshaderside->Bind(context);
					front = true;
				}
			} else {
				if (front) {
					_vshader->Bind(context);
					front = false;
				}
			}
		}

		void Release() {
			DebugPrint(std::string("\t Pipeline::Release() ... \n"));

			_vshader->Reset();
			_vshaderside->Reset();
			_pixelshader->Reset();
		}
	private:
		std::shared_ptr<VertexShader<DirectX::VertexPositionTextureInstanced>> _vshader;
		std::shared_ptr<VertexShader<DirectX::VertexPositionTextureInstanced>> _vshaderside;
		std::shared_ptr<PixelShader> _pixelshader;
		bool front = true;
		
	};

}
