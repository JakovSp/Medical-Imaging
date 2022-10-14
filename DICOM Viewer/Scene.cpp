
//
//	Scene.cpp: 
//	Manages scene objects 
//	
//  © VanityXS - DirectX 11.2 Student Engine. Zoraja Consulting d.o.o. All rights reserved.
//	vJan22 
//

#include "pch.h"

#include "Scene.h"

#include <Engine\Models\Mesh.h>

#include <Engine\Models\Basic Shapes\Triangles.h>
#include <Engine\Models\Basic Shapes\Cubes.h>
#include <Engine\Models\Basic Shapes\Pyramids.h>

#include "DICOMLoader.h"
#include "Wireframe.h"
#include "ObjectAlignedSlices.h"
#include "Textures.h"

using namespace std;
using namespace vxe;
using namespace vxe::med;

using namespace concurrency;
using namespace DirectX;
using namespace Windows::System;

Scene::Scene()
{
	DebugPrint(string("\t Scene::Ctor... \n"));
}

void Scene::LoadAssets(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {
	DebugPrint(string("\t Scene::LoadAssets()... \n"));

 	DICOMLoader DICOMdata(L"Assets\\Ankle");
	
	auto device = vanitycore->GetD3DDevice();
	auto world = make_shared<WorldTransforms>(device);

	// _texArray = make_shared<SceneTexture<Texture2D>>(DICOMdata.LoadTextureArray(CorticalBone, tasks, vanitycore));
	_tex3D = make_shared<SceneTexture<Texture3D>>(DICOMdata.LoadTexture3D(CorticalBone, tasks, vanitycore));
	_VAvolumetric = make_shared<VASBVolume>(device, 512, 512, 150, 1.0f, 1.0f, 1.0f);
	auto slicepolygon = make_shared<BoxSlice>(1);
	tasks.push_back(slicepolygon->CreateAsync(device));
	_VAvolumetric->SetMesh(slicepolygon);
	_VAvolumetric->SetTexture(_tex3D);
	_VAvolumetric->SetWorld(world);

	shared_ptr<Mesh<VertexPosition, uint16_t>> boxmesh;
	boxmesh = make_shared<Box>(512, 512, 150, 1.0f, 1.0f, 1.0f);
	tasks.push_back(boxmesh->CreateAsync(device));
	_box = make_shared<SceneObject<VertexPosition, uint16_t>>();
	_box->SetMesh(boxmesh);
	_box->SetWorld(world);

	// auto meshslices = make_shared<ObjectAlignedSlices>(512, 512, 150, 1.0f, 1.0f, 1.0f);
	// tasks.push_back(meshslices->CreateAsync(device));
	// _volumetric = make_shared<OASBVolume>();
	// _volumetric->SetMesh(meshslices);
	// _volumetric->SetTexture(_texArray);
	// _volumetric->SetWorld(world);

	// shared_ptr<Mesh<VertexPosition, uint16_t>> trimesh;
	// trimesh = DICOMdata.LoadWireframeMesh(CorticalBone, tasks, vanitycore);
	// _trisurface = make_shared<SceneObject<VertexPosition, uint16_t>>();
	// _trisurface->SetMesh(trimesh);
	// _trisurface->SetWorld(world);

	// shared_ptr<Mesh<VertexPosition, uint16_t>> pointmesh;
	// pointmesh = DICOMdata.LoadPointCloud(CorticalBone, tasks, vanitycore);
	// _pointcloud = make_shared<SceneObject<VertexPosition, uint16_t>>();
	// _pointcloud->SetMesh(pointmesh);
	// _pointcloud->SetWorld(world);

}

void Scene::SetTextures(shared_ptr<VanityCore>& vanitycore) {
	if (_texArray) {
		_texArray->Initialize(vanitycore);
		_texArray->Bind(vanitycore);
	}

	if (_tex3D) {
		_tex3D->Initialize(vanitycore);
		_tex3D->Bind(vanitycore);
	}
}

void Scene::SetCamera(shared_ptr<VanityCore>& vanitycore)
{
	DebugPrint(string("\t Scene::SetCamera()... \n"));

	_camera.Initialize(vanitycore);
	_camera.Bind(vanitycore);
}

void Scene::Update(DX::StepTimer const& timer, shared_ptr<VanityCore>& vanitycore, InputController^ ic)
{
	__Once(DebugPrint(string("Scene::Update() ...\n")));

	float _rps = DirectX::XM_PI / 6.0f ;

	XMFLOAT2 prevpointer{0.0f, 0.0f};
	float dx = 0, dy = 0, dz = 0;

	bool update = false;
	if (ic->IsLeftButtonPressed()) {
		prevpointer = _pointer;
		_pointer = ic->GetPointerPosition();
		dx = (prevpointer.x - _pointer.x)*0.005;
		dy = (prevpointer.y - _pointer.y)*0.005;
		update = true;
	}

	_pointer = ic->GetPointerPosition();

	if (ic->IsKeyDown(VirtualKey::W)) {
		dz = -0.05;
		update = true;
	}

	if (ic->IsKeyDown(VirtualKey::S)) {
		dz = 0.05;
		update = true;
	}

	if(update){
		_camera.Update(vanitycore, dz, dx, dy);
	}
}

void Scene::DrawTriMesh(std::shared_ptr<VanityCore>& vanitycore, bool indexed) {
	__Once(DebugPrint(string("Scene::Draw() ...\n")));

	auto device = vanitycore->GetD3DDevice();
	auto context = vanitycore->GetD3DDeviceContext();

	_box->Bind(context);
	_box->Draw(context);
}

void Scene::DrawPointCloud(std::shared_ptr<VanityCore>& vanitycore, bool indexed) {
	__Once(DebugPrint(string("Scene::Draw() ...\n")));

	auto device = vanitycore->GetD3DDevice();
	auto context = vanitycore->GetD3DDeviceContext();

	// _pointcloud->Bind(context);
	// _pointcloud->Draw(context, false);
}

void Scene::DrawVAVolumetric(shared_ptr<VanityCore>& vanitycore, bool indexed) {
	__Once(DebugPrint(string("Scene::Draw() ...\n")));

	auto device = vanitycore->GetD3DDevice();
	auto context = vanitycore->GetD3DDeviceContext();

	// vanitycore->SetBlenderState();
	_VAvolumetric->SwitchSamplingDirection(device, &_camera.GetView());
	_VAvolumetric->Draw(context);
}

void Scene::DrawOAVolumetric(shared_ptr<VanityCore>& vanitycore, bool indexed) {
	__Once(DebugPrint(string("Scene::Draw() ...\n")));

	auto device = vanitycore->GetD3DDevice();
	auto context = vanitycore->GetD3DDeviceContext();

	// vanitycore->SetBlenderState();
	_volumetric->SwitchSamplingDirection(device, &_camera.GetView());
	_volumetric->Draw(context);

}

// void Scene::Render(shared_ptr<VanityCore>& vanitycore) {
// 	__Once(DebugPrint(string("Scene::Render() ...\n")));
// 	DrawVolumetric(vanitycore, true);
// }

void Scene::Release()
{
	DebugPrint(string("\t Scene::Release()... \n"));

	// _PCobject->Release();
	// _MCobject->Release();
	// _pointcloud->Release();
	// _volumetricslice->Release();
	_camera.Release();
}