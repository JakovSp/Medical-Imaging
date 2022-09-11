
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

using namespace std;
using namespace vxe;
using namespace vxe::med;

using namespace concurrency;
using namespace DirectX;

Scene::Scene()
{
	DebugPrint(string("\t Scene::Ctor... \n"));
}


void Scene::LoadAssets(vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore)
{
	DebugPrint(string("\t Scene::LoadAssets()... \n"));

	DICOMLoader DICOMdata(L"Assets\\Ankle");
	
	auto device = vanitycore->GetD3DDevice();

	shared_ptr<Mesh<VertexPosition, uint16_t>> pointcloud;
	shared_ptr<Mesh<VertexPosition, uint16_t>> trimesh;
	pointcloud = make_shared<PointCloud<VertexPosition, uint16_t>>();
	trimesh = make_shared<Wireframe<VertexPosition, uint16_t>>();

	tasks.push_back(pointcloud->CreateAsync(device, DICOMdata.LoadPointCloud()));
	// tasks.push_back(trimesh->CreateAsync(device, DICOMdata.LoadWireframeMesh()));
	auto world = make_shared<WorldTransforms>(device);

	_PCobject = make_shared<SceneObject<VertexPosition, uint16_t>>();
	_PCobject->SetMesh(pointcloud);
	_PCobject->SetWorld(world);

	//_MCobject = make_shared<SceneObject<VertexPosition, uint16_t>>();
	//_MCobject->SetMesh(trimesh);
	//_MCobject->SetWorld(world);
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
	_PCobject->GetWorld()->RotateY(angle);
	// _MCobject->GetWorld()->RotateY(angle);
}

void Scene::Draw(shared_ptr<VanityCore>& vanitycore, bool indexed)
{
	__Once(DebugPrint(string("Scene::Draw() ...\n")));

	auto context = vanitycore->GetD3DDeviceContext();

	_PCobject->Bind(context);
	_PCobject->Draw(context, false);
	//_MCobject->Bind(context);
	//_MCobject->Draw(context, false);
}

void Scene::Render(shared_ptr<VanityCore>& vanitycore)
{
	__Once(DebugPrint(string("Scene::Render() ...\n")));

	Draw(vanitycore, true);
}

void Scene::Release()
{
	DebugPrint(string("\t Scene::Release()... \n"));

	_PCobject->Release();
	// _MCobject->Release();
	_camera.Release();
}