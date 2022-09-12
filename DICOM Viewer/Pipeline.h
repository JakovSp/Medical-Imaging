
#pragma once

//
//	Pipeline.h: 
//	Manages pipeline stages
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include <Engine\Third Party\DirectX Tool Kit\VertexTypes.h>

#include <Engine\Pipeline Stages\Shaders\Vertex Shader.h>
#include <Engine\Pipeline Stages\Shaders\Pixel Shader.h>

namespace vxe {

	class Pipeline {
		using VertexType_t = DirectX::VertexPositionTexture3;

	public:
		Pipeline();

		void LoadShaders(std::vector<concurrency::task<void>>&, std::shared_ptr<VanityCore>&);
		void BindShaders(std::shared_ptr<VanityCore>&);

		//void SetRenderTargets(std::shared_ptr<VanityCore>&);
		
		void Release();

	private:
		std::shared_ptr<VertexShader<VertexType_t>> _vertexshader;
		std::shared_ptr<PixelShader> _pixelshader;

		std::wstring _vsfilename = L"Vertex Shader.cso";
		std::wstring _psfilename = L"Pixel Shader.cso";
	};

}