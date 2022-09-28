
#pragma once

//
//	Pipeline.h: 
//	Manages pipeline stages
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"
#include <filesystem>
#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>

#include <Engine\Pipeline Stages\Shaders\Vertex Shader.h>
#include <Engine\Pipeline Stages\Shaders\Pixel Shader.h>

namespace vxe {

	template<typename VertexType_t>
	class Pipeline {
	public:
		Pipeline() {
			DebugPrint(std::string("\t Pipeline::Ctor... \n"));
		}
		Pipeline(std::wstring vsfilename, std::wstring psfilename) :
			_vsfilename(vsfilename), _psfilename(psfilename) {
			_installfolderpath = Windows::ApplicationModel::Package::Current->InstalledLocation->Path->Data();
			DebugPrint(std::string("\t Pipeline::Ctor... \n"));
		}

		void LoadShaders(std::vector<concurrency::task<void>>& tasks, std::shared_ptr<VanityCore>& vanitycore) {
			DebugPrint(string("\t Pipeline::LoadShaders() ... \n"));

			auto device = vanitycore->GetD3DDevice();
			
			_vertexshader = make_shared<VertexShader<VertexType_t>>();
			tasks.push_back(_vertexshader->CreateAsync(device, _installfolderpath/_vsfilename));

			_pixelshader = make_shared<PixelShader>();
			tasks.push_back(_pixelshader->CreateAsync(device, _installfolderpath/_psfilename));
		}

		void BindShaders(std::shared_ptr<VanityCore>& vanitycore) {
			auto context = vanitycore->GetD3DDeviceContext();

			_vertexshader->Bind(context);
			_pixelshader->Bind(context);
		}

		void Release() {
			DebugPrint(string("\t Pipeline::Release() ... \n"));

			_vertexshader->Reset();
			_pixelshader->Reset();
		}

	private:
		std::shared_ptr<VertexShader<VertexType_t>> _vertexshader;
		std::shared_ptr<PixelShader> _pixelshader;
		std::wstring _vsfilename;
		std::wstring _psfilename;
		std::filesystem::path _installfolderpath;
	};

}