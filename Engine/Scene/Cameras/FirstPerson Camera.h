
#pragma once

//
//	FirstPerson OrbitalCamera.h
//	Encapsulates basic camera movements and rotations
//
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22
//

#include "pch.h"

#include "..\Input\Input Controller.h"

#include "Camera Base.h"

#include "..\..\Core\Common\StepTimer.h"

namespace vxe {

	class FirstPersonCamera : public CameraBase {

	public:
		FirstPersonCamera(_In_ ID3D11Device2*);

		void Update(InputController^, DX::StepTimer const&);
		
		void ProcessInput(InputController^, float);
		DirectX::XMFLOAT3 ProcessKeyboard(InputController^, float);
		void ProcessPointer(InputController^, float);

	private:
		// Rotations about x and y axis 
		float _alfa;
		float _beta;
		DirectX::XMMATRIX _rotation;
		DirectX::XMFLOAT2 _lastpointerposition;
		DirectX::XMFLOAT2 _currentpointerposition;

		static const DirectX::XMFLOAT3 _movevelocity;
		static const DirectX::XMFLOAT2 _rotationvelocity;
		static const float _debounce;
	};

}