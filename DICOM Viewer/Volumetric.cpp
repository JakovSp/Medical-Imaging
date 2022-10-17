#include "pch.h"

#include <Engine\Scene\Scene Object.h>
#include "Volumetric.h"

using namespace std;
using namespace vxe;
using namespace DirectX;

void OASBVolume::SetMesh(std::shared_ptr<ObjectAlignedSlices> mesh) {
	_meshslices = mesh;
	_mesh = mesh;
}

void OASBVolume::SetWorld(shared_ptr<WorldTransforms>& world) { _world = world; }
shared_ptr<WorldTransforms> OASBVolume::GetWorld() { return _world; }

void OASBVolume::SwitchSamplingDirection(_In_ ID3D11Device2* device, XMFLOAT4X4* view) {
	XMMATRIX view_to_world = GetInverseMatrix(XMLoadFloat4x4(view));
	XMMATRIX world_to_model = GetInverseMatrix(XMLoadFloat4x4(&GetWorld()->GetWorld()));
	XMMATRIX view_to_model = view_to_world * world_to_model;
	XMVECTOR v_direction_m = XMVector4Transform(FXMVECTOR{ 0, 0, 1, 0 }, view_to_world);
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

void OASBVolume::Draw(_In_ ID3D11DeviceContext2* context, bool indexed) {
	_samplingbuffer->Update(context, _samplingmatrix);
	_samplingbuffer->Bind(context, ProgrammableStage::VertexShaderStage, _samplingbufferid);
	Bind(context);
	_meshslices->Draw(context, _orientation);
}

void OASBVolume::SetTexture(shared_ptr<SceneTexture<Texture2D>> texturearray) {
	_textureslices = texturearray;
}

void OASBVolume::BindTexture(shared_ptr<VanityCore>& vanitycore) {
	_textureslices->Bind(vanitycore);
}

VASBVolume::VASBVolume(_In_ ID3D11Device2* device, int32_t samples_x, uint32_t samples_y, uint32_t samples_z, float dx, float dy, float dz) {
	float width = dx * samples_x;
	float height = dy * samples_y;
	float depth = dz * samples_z;

	// normalize
	float& largest = width > height ? (width > depth ? width : depth) : (height > depth? height : depth);

	float norm_x = width/largest;
	float norm_y = height/largest;
	float norm_z = depth/largest;

	_perframedata.vecVertices[0] = XMFLOAT4(-norm_x / 2.0f, norm_y / 2.0f, -norm_z / 2.0f, 0.0f);
	_perframedata.vecVertices[1] = XMFLOAT4(norm_x / 2.0f, norm_y / 2.0f, -norm_z / 2.0f, 0.0f);
	_perframedata.vecVertices[2] = XMFLOAT4(-norm_x / 2.0f, -norm_y / 2.0f, -norm_z / 2.0f, 0.0f);
	_perframedata.vecVertices[3] = XMFLOAT4(-norm_x / 2.0f, norm_y / 2.0f, norm_z / 2.0f, 0.0f);
	_perframedata.vecVertices[4] = XMFLOAT4(norm_x / 2.0f, norm_y / 2.0f, norm_z / 2.0f, 0.0f);
	_perframedata.vecVertices[5] = XMFLOAT4(norm_x / 2.0f, -norm_y / 2.0f, -norm_z / 2.0f, 0.0f);
	_perframedata.vecVertices[6] = XMFLOAT4(-norm_x / 2.0f, -norm_y / 2.0f, norm_z / 2.0f, 0.0f);
	_perframedata.vecVertices[7] = XMFLOAT4(norm_x / 2.0f, -norm_y / 2.0f, norm_z / 2.0f, 0.0f);
 	_perframedata.frontIndex = 0;

	auto diagonal = XMVectorSubtract(
		XMLoadFloat4(&_perframedata.vecVertices[7]),
		XMLoadFloat4(&_perframedata.vecVertices[0]) );

	XMVECTOR diaglength = XMVector3Length(diagonal);
	_perframedata.samplingrate = diaglength.m128_f32[0]/largest;

	_constantbuffer = make_shared<ConstantBuffer<VASConstantData>>(device, &_constantdata);
}

void VASBVolume::SwitchSamplingDirection(_In_ ID3D11Device2* device, DirectX::XMFLOAT4X4* view){
	XMMATRIX world_to_view =  XMLoadFloat4x4(view);
	XMMATRIX model_to_world = XMLoadFloat4x4(&GetWorld()->GetWorld());
	XMMATRIX model_to_view = model_to_world * world_to_view;

	// XMMATRIX view_to_world = GetInverseMatrix(  XMLoadFloat4x4(view));
	// XMMATRIX world_to_model = GetInverseMatrix(  XMLoadFloat4x4(&GetWorld()->GetWorld()));
	// XMMATRIX view_to_model = view_to_world * world_to_model;

	XMMATRIX view_to_model = GetInverseMatrix( model_to_view );
	XMVECTOR viewdir_m = XMVector4Transform(FXMVECTOR{ 0, 0, 1, 1 }, XMMatrixTranspose(view_to_model));
	viewdir_m = XMVector3Normalize(viewdir_m);
 	XMStoreFloat4(&_perframedata.vecView, viewdir_m);

	float maxdot = 0.0f;
	XMVECTOR v;
	// sort vertices by dot product of a vertex by a v_direction_m
	for (size_t i = 0; i < 8; i++) {
		v = XMVector4Dot(viewdir_m, XMLoadFloat4(&_perframedata.vecVertices[i]));
		if (v.m128_f32[0] > maxdot) {
			_perframedata.frontIndex = i;
			maxdot = v.m128_f32[0];
		}
	}

	auto back_m = XMLoadFloat4(&_perframedata.vecVertices[endOf[_perframedata.frontIndex]]);
	back_m.m128_f32[3] = 1.0f;
	auto back_v = XMVector4Transform(back_m, XMMatrixTranspose(model_to_view));
	auto length = XMVector3Length(back_m);
	_perframedata.dBack = length.m128_f32[0];
	_perframebuffer = make_shared<ConstantBuffer<VASPerFrameData>>(device, &_perframedata);
}

void VASBVolume::SetTexture(std::shared_ptr<SceneTexture<Texture3D>> texturearray){
	_texture3D = texturearray;
}

void VASBVolume::BindTexture(std::shared_ptr<VanityCore>& vanitycore){
	_texture3D->Bind(vanitycore);
}

void VASBVolume::Draw(_In_ ID3D11DeviceContext2* context, bool indexed){
	_constantbuffer->Update(context, _constantdata);
	_constantbuffer->Bind(context, ProgrammableStage::VertexShaderStage, 3);
	_perframebuffer->Update(context, _perframedata);
	_perframebuffer->Bind(context, ProgrammableStage::VertexShaderStage, 4);
	Bind(context);
	_slicepolygon->DrawIndexedInstanced(context);
}

void VASBVolume::SetMesh(std::shared_ptr<BoxSlice> mesh) {
	_mesh = mesh;
	_slicepolygon = mesh;
}
