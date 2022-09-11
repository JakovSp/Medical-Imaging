
#pragma once

//
//	Camera Base.h
//	Encapsulates basic camera elements: position, view and projection
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "..\Transforms\Position.h"
#include "..\Transforms\View Transform.h"
#include "..\Transforms\Projection Transform.h"

namespace vxe {

	class CameraBase {

	public:
		CameraBase(_In_ ID3D11Device2*);

		void InitializeView(DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR);
		void SetView();

		auto GetEye() { return _eye.GetPosition(); }
		auto GetLookAt() { return DirectX::XMLoadFloat3(&_lookat); }
		auto GetUp() { return DirectX::XMLoadFloat3(&_up); }

		void SetProjection(DirectX::CXMMATRIX orientation, float a, float b, float zn, float zf)
		{
			_projection.SetProjection(orientation, a, b, zn, zf);
		}

		void UpdateEye(_In_ ID3D11DeviceContext2* context) { _eye.Update(context); }
		void UpdateView(_In_ ID3D11DeviceContext2* context) { _view.Update(context); }
		void UpdateProjection(_In_ ID3D11DeviceContext2* context) { _projection.Update(context); }

		void BindEye(_In_ ID3D11DeviceContext2* context,
			ProgrammableStage shader = ProgrammableStage::PixelShaderStage,
			uint32_t slot = 0,
			uint32_t count = 1)
		{
			_eye.GetConstantBuffer()->Bind(context, shader, slot, count);
		}

		void BindView(_In_ ID3D11DeviceContext2* context,
			ProgrammableStage shader = ProgrammableStage::VertexShaderStage,
			uint32_t slot = 0,
			uint32_t count = 1)
		{
			_view.GetConstantBuffer()->Bind(context, shader, slot, count);
		}

		void BindProjection(_In_ ID3D11DeviceContext2* context,
			ProgrammableStage shader = ProgrammableStage::VertexShaderStage,
			uint32_t slot = 0,
			uint32_t count = 1)
		{
			_projection.GetConstantBuffer()->Bind(context, shader, slot, count);
		}

		void Reset();
	
	protected:
		Position _eye;
		DirectX::XMFLOAT3 _lookat;
		DirectX::XMFLOAT3 _up;

		ViewTransform _view;
		ProjectionTransform _projection;
	};
}