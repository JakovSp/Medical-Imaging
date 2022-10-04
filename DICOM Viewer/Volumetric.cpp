#include "pch.h"
#include <Engine\Scene\Scene Object.h>
#include "Volumetric.h"

using namespace std;
using namespace vxe;
using namespace DirectX;

void Volumetric::SetMesh(std::shared_ptr<ObjectAlignedSlices> mesh) {
	_meshslices = mesh;
	_mesh = mesh;
}

void Volumetric::SetWorld(shared_ptr<WorldTransforms>& world) { _world = world; }
shared_ptr<WorldTransforms> Volumetric::GetWorld() { return _world; }

void Volumetric::SwitchSamplingDirection(_In_ ID3D11Device2* device, XMFLOAT4X4* view) {
	XMMATRIX view_to_world = GetInverseMatrix(XMLoadFloat4x4(view));
	XMMATRIX world_to_model = GetInverseMatrix(XMLoadFloat4x4(&GetWorld()->GetWorld()));
	XMMATRIX view_to_model = view_to_world * world_to_model;
	XMVECTOR v_direction_m = XMVector4Transform(FXMVECTOR{ 0, 0, 1, 0 }, view_to_model);
	XMFLOAT4 direction;
	XMStoreFloat4(&direction, v_direction_m);
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

	_samplingbuffer = make_shared<ConstantBuffer<XMFLOAT4X4>>(device, &_samplingmatrix);
}

void Volumetric::Draw(_In_ ID3D11DeviceContext2* context, bool indexed) {
	_samplingbuffer->Update(context, _samplingmatrix);
	_samplingbuffer->Bind(context, ProgrammableStage::VertexShaderStage, _samplingbufferid);
	Bind(context);
	_meshslices->Draw(context, _orientation);
}


void Volumetric::SetTexture(shared_ptr<SceneTexture<Texture2D>> texturearray) {
	_textureslices = texturearray;
}
void Volumetric::BindTexture(shared_ptr<VanityCore>& vanitycore) {
	_textureslices->Bind(vanitycore);
}
