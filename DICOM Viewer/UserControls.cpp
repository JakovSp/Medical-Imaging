#include "pch.h"
#include "UserControls.h"

using namespace std;
using namespace vxe;
using namespace DirectX;

void UserControls::ScaleRotateTranslate(	shared_ptr<WorldTransforms>& worldobject, 
									XMFLOAT3 scale, 
									XMFLOAT3 orient, 
									XMFLOAT3 position) {
	auto scaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	auto translation = XMMatrixTranslation(position.x, position.y, position.z);
	auto rotation = XMMatrixMultiply(	XMMatrixMultiply(	XMMatrixRotationX(orient.x), 
															XMMatrixRotationY(orient.y)), 
															XMMatrixRotationZ(orient.z));
	
	auto world = XMMatrixMultiply(XMMatrixMultiply(	scaling,
													rotation),
													translation);
	auto dlrow = GetInverseMatrix(world);

	XMFLOAT4X4 storedworld;
	XMFLOAT4X4 storeddlrow;
	XMStoreFloat4x4(&storedworld, XMMatrixTranspose(world));
	XMStoreFloat4x4(&storeddlrow, dlrow);
	worldobject->SetWorld(storedworld);
	worldobject->SetDlrow(storeddlrow);
}

void UserControls::MotionTransform(Camera& camera, XMFLOAT4X4& storedworld, XMFLOAT4X4& storeddlrow) {
	XMMATRIX world_to_eye = XMMatrixTranspose( XMLoadFloat4x4(&camera.GetView()->GetValue()) );
	XMMATRIX eye_to_world = GetInverseMatrix(world_to_eye);
		
	XMMATRIX S = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX C = XMMatrixTranslation(1.0f, 1.0f, 1.0f);
	XMMATRIX T = XMMatrixTranslation(_position[0].x, _position[0].y, _position[0].z);
	XMMATRIX R = XMMatrixRotationX(_orient[0].x) * XMMatrixRotationY(_orient[0].y) * XMMatrixRotationZ(_orient[0].z);

	XMMATRIX weTew = world_to_eye * T * eye_to_world;
	XMMATRIX transform = S * R * weTew;

	auto dlrow = GetInverseMatrix(transform);

	XMStoreFloat4x4(&storedworld, XMMatrixTranspose(transform));
	XMStoreFloat4x4(&storeddlrow, dlrow);
}

void UserControls::Update(const DX::StepTimer& timer, InputController^ IC) {
	bool update = false;
	__Once(DebugPrint(string("Scene::Update() ...\n")));

	// Controls:
	float floating_increment = 0.01;
	if (IC->IsKeyDown(Windows::System::VirtualKey::Control)) {
		_prevpointer = _currpointer;
		_currpointer = IC->GetPointerPosition();

		for (size_t i = 0; i < _orient.size(); i++) {
			_orient[i].x -= (_currpointer.y - _prevpointer.y) * 0.01;
			_orient[i].y -= (_currpointer.x - _prevpointer.x) * 0.01;
		}

		update = true;
	}
	_currpointer = IC->GetPointerPosition();
	
	if (IC->IsKeyDown(Windows::System::VirtualKey::W)) {
		for (size_t i = 0; i < _position.size(); i++) {
			_position[i].z += floating_increment;
		}

		update = true;
	}
	if (IC->IsKeyDown(Windows::System::VirtualKey::S)) {
		for (size_t i = 0; i < _position.size(); i++) {
			_position[i].z -= floating_increment;
		}

		update = true;
	}
	if (IC->IsKeyDown(Windows::System::VirtualKey::D)) {
		for (size_t i = 0; i < _position.size(); i++) {
			_position[i].x -= floating_increment;
		}

		update = true;
	}
	if (IC->IsKeyDown(Windows::System::VirtualKey::A)) {
		for (size_t i = 0; i < _position.size(); i++) {
			_position[i].x += floating_increment;
		}

		update = true;
	}
	if (IC->IsKeyDown(Windows::System::VirtualKey::Space)) {
		for (size_t i = 0; i < _position.size(); i++) {
			_position[i].y -= floating_increment;
		}

		update = true;
	}
	if (IC->IsKeyDown(Windows::System::VirtualKey::C)) {
		for (size_t i = 0; i < _position.size(); i++) {
			_position[i].y += floating_increment;
		}

		update = true;
	}

	if (update) {
		MotionTransform();
	}
}
