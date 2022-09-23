
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

Scene::Scene()
{
	DebugPrint(string("\t Scene::Ctor... \n"));
}

void Scene::LoadAssets(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {
	DebugPrint(string("\t Scene::LoadAssets()... \n"));

 	DICOMLoader DICOMdata(L"Assets\\Ankle");
	
	auto device = vanitycore->GetD3DDevice();
	auto world = make_shared<WorldTransforms>(device);

	_texArray = make_shared<SceneTexture<Texture2D>>(DICOMdata.LoadTextureArray(tasks, vanitycore));
	shared_ptr<Mesh<VertexPositionTextureInstanced, uint16_t>> slice;
	slice = make_shared<ObjectAlignedSlices<VertexPositionTextureInstanced, uint16_t>>(_texArray->GetTexture()->GetDepth());
	tasks.push_back(slice->CreateAsync(device));
	_volumetricslice = make_shared<SceneObject<VertexPositionTextureInstanced, uint16_t>>();
	_volumetricslice->SetMesh(slice);
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
	if (_texArray) {
		_texArray->Initialize(vanitycore);
		_texArray->Bind(vanitycore);
	}

}

void Scene::SetCamera(shared_ptr<VanityCore>& vanitycore)
{
	DebugPrint(string("\t Scene::SetCamera()... \n"));

	_camera.Initialize(vanitycore);
	_camera.Bind(vanitycore);
}

void Scene::Update(DX::StepTimer const& timer)
{
	__Once(DebugPrint(string("Scene::Update() ...\n")));

	auto angle = _animation.Angle(static_cast<float>(timer.GetElapsedSeconds()));
	_volumetricslice->GetWorld()->RotateY(angle);
	//	_PCobject->GetWorld()->RotateY(angle);
	// _MCobject->GetWorld()->RotateY(angle);
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
	shared_ptr<ConstantBuffer<uint32_t>> instancecount;
	instancecount = make_shared<ConstantBuffer<uint32_t>>(device);
	instancecount->Update(context, _volumetricslice->GetMesh()->GetInstanceCount());
	instancecount->Bind(context, ProgrammableStage::VertexShaderStage, 3);

	 vanitycore->SetBlenderState();
	_volumetricslice->Bind(context);
	_volumetricslice->DrawInstanced(context);

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