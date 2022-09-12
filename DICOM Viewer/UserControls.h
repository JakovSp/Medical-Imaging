#pragma once

#include <Engine\Scene\Input\Input Controller.h>
#include <Engine\Scene\Scene Object.h>
#include "Camera.h"

class UserControls {
public:
	UserControls(){}
	void ScaleRotateTranslate(std::shared_ptr<WorldTransforms>& worldobject,
								DirectX::XMFLOAT3 scale,
								DirectX::XMFLOAT3 orient,
								DirectX::XMFLOAT3 position);
	void Update(const DX::StepTimer& timer, InputController^ IC);
	void MotionTransform(Camera& camera, XMFLOAT4X4& storedworld, XMFLOAT4X4& storeddlrow);
private:
	std::vector<DirectX::XMFLOAT3> _position;
	std::vector<DirectX::XMFLOAT3> _orient;
	DirectX::XMFLOAT2 _prevpointer{ 0,0 };
	DirectX::XMFLOAT2 _currpointer{ 0,0 };
	DirectX::XMFLOAT2 _diff{ 0,0 };
};
