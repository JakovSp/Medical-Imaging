
#pragma once

//
//	Textures.h: 
//	Manages textures
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include <Engine\Pipeline Resources\Textures\Texture2D.h>
#include <Engine\Pipeline Resources\Textures\Texture3D.h>

namespace vxe {

	template<typename T>
	class SceneTexture {

	public:
		SceneTexture() {
			DebugPrint(string("\t Textures::Ctor... \n"));
		};
		SceneTexture(std::shared_ptr<T> texture) :
			_texture(texture)
		{
			DebugPrint(string("\t Textures::Ctor... \n"));
		};

		void Load(std::vector<concurrency::task<void>>& tasks, std::wstring filename){
			DebugPrint(string("\t Textures::Load() ... \n"));
			tasks.push_back(_texture->CreateAsync(filename));
		}

		void Load(std::vector<concurrency::task<void>>& tasks, const uint8_t* data){
			DebugPrint(string("\t Textures::Load() ... \n"));
			tasks.push_back(_texture->CreateAsyncFromMemory(data));
		}

		void Initialize(std::shared_ptr<VanityCore>& vanitycore) {
			DebugPrint(string("\t Textures::Initialize() ...\n"));

			_texture->CreateSamplerState();
			_texture->CreateShaderResourceView();
		}

		void Bind(std::shared_ptr<VanityCore>& vanitycore) {
			auto context = vanitycore->GetD3DDeviceContext();

			_texture->BindSamplerState(context, _samplerregister);
			_texture->BindShaderResourceView(context, ProgrammableStage::PixelShaderStage, _textureregister);
		}

		std::shared_ptr<T> GetTexture() { return _texture; }

		void Release() {
			_texture->Reset();
		};

	private:
		std::shared_ptr<T> _texture;

		uint32_t _samplerregister = 0;
		uint32_t _textureregister = 0;
	};

}
