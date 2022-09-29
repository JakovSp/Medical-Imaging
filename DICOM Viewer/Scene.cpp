
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
#include "PointCloud.h"
#include "Wireframe.h"
#include "Quad.h"
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

	_texture3D = make_shared<SceneTexture<Texture3D>>(DICOMdata.LoadTexture3D(tasks, vanitycore));
	shared_ptr<Mesh<VertexPositionTexture3, uint16_t>> quad;
	quad = make_shared<Quad<VertexPositionTexture3, uint16_t>>();
	tasks.push_back(quad->CreateAsync(device));
	_volumetricslice = make_shared<SceneObject<VertexPositionTexture3, uint16_t>>();
	_volumetricslice->SetMesh(quad);
	_volumetricslice->SetWorld(world);

	// shared_ptr<Mesh<VertexPosition, uint16_t>> trimesh;
	// trimesh = make_shared<Wireframe<VertexPosition, uint16_t>>();
	// tasks.push_back(trimesh->CreateAsync(device, DICOMdata.LoadWireframeMesh()));
	// _trisurface = make_shared<SceneObject<VertexPosition, uint16_t>>();
	// _trisurface->SetMesh(trimesh);
	// _trisurface->SetWorld(world);

	// shared_ptr<Mesh<VertexPosition, uint16_t>> pointmesh;
	// pointmesh = make_shared<PointCloud2<VertexPosition, uint16_t>>();
	// tasks.push_back(pointmesh->CreateAsync(device, DICOMdata.LoadPointCloud()));
	// _pointcloud = make_shared<SceneObject<VertexPosition, uint16_t>>();
	// _pointcloud->SetMesh(pointmesh);
	// _pointcloud->SetWorld(world);

}

void Scene::SetTextures(shared_ptr<VanityCore>& vanitycore) {
	if (_texture3D) {
		_texture3D->Initialize(vanitycore);
		_texture3D->Bind(vanitycore);
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

	XMFLOAT2 prevpointer;
	bool update = false;
	if (ic->IsLeftButtonPressed()) {
		prevpointer = _pointer;
		_pointer = ic->GetPointerPosition();
		update = true;
	}

	_pointer = ic->GetPointerPosition();

	if(update){
		_camera.move.x += (prevpointer.x - _pointer.x)*0.005;
		_camera.move.y += (prevpointer.y - _pointer.y)*0.005;
		_camera.Update(vanitycore);
	}

}

void Scene::DrawTriMesh(std::shared_ptr<VanityCore>& vanitycore, bool indexed) {
	__Once(DebugPrint(string("Scene::Draw() ...\n")));

	auto device = vanitycore->GetD3DDevice();
	auto context = vanitycore->GetD3DDeviceContext();

	_trisurface->Bind(context);
	_trisurface->Draw(context, false);
}

void Scene::DrawPointCloud(std::shared_ptr<VanityCore>& vanitycore, bool indexed) {
	__Once(DebugPrint(string("Scene::Draw() ...\n")));

	auto device = vanitycore->GetD3DDevice();
	auto context = vanitycore->GetD3DDeviceContext();

	_pointcloud->Bind(context);
	_pointcloud->Draw(context, false);
}

void Scene::DrawVolumetric(shared_ptr<VanityCore>& vanitycore, bool indexed) {
	__Once(DebugPrint(string("Scene::Draw() ...\n")));

	auto device = vanitycore->GetD3DDevice();
	auto context = vanitycore->GetD3DDeviceContext();

	shared_ptr<ConstantBuffer<TransferZ>> instancebuffer;
	instancebuffer = make_shared<ConstantBuffer<TransferZ>>(device);

	vanitycore->SetBlenderState();
 	for (float z = -0.5f; z < 0.5f; z += 1.0f / _texture3D->GetTexture()->GetTexDepth()) {
 		TransferZ sliceindex;
 		sliceindex.vertZ = z;
 		sliceindex.texZ = (z + 0.5f) / 1.0f;
 		instancebuffer->Update(context, sliceindex);
 		instancebuffer->Bind(context, ProgrammableStage::VertexShaderStage, 3);
 
		_volumetricslice->Bind(context);
		_volumetricslice->Draw(context);
 	}

}

void Scene::Render(shared_ptr<VanityCore>& vanitycore) {
	__Once(DebugPrint(string("Scene::Render() ...\n")));

	DrawVolumetric(vanitycore, true);
}

void Scene::Release()
{
	DebugPrint(string("\t Scene::Release()... \n"));

	// _PCobject->Release();
	// _MCobject->Release();
	_pointcloud->Release();
	_volumetricslice->Release();
	_camera.Release();
}