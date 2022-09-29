
#pragma once

//
//	Renderer.h
//  Defining a 3D renderer
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include <Engine\Core\Content\RendererBase3D.h>
#include <Engine\Scene\Input\Input Controller.h>

#include "Scene.h"
#include "Pipeline.h"

namespace vxe
{
	class Renderer : public RendererBase3D {

	public:
		Renderer(); 

		virtual void CreateDeviceResources() override;
		virtual void CreateWindowResources() override;
		virtual void ReleaseDeviceResources() override;
	
		virtual void Render() override;
		virtual void Update(const DX::StepTimer&) override;

	private:
		std::shared_ptr<Scene> _scene;
		std::shared_ptr<Pipeline<DirectX::VertexPositionTexture3>> _volumetricpipeline;
		std::shared_ptr<Pipeline<DirectX::VertexPosition>> _wireframepipeline;
		SceneObjectType _objectbound;
	};

}