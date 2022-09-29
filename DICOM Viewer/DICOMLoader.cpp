#include "pch.h"

#include "DICOMLoader.h"
#include "PointCloud.h"
#include "Wireframe.h"
#include <ctime>

#include <DICOM Reader/Convert/Geometry/Geometry.h>
#include <DICOM Reader/Convert/Geometry/Volume.h>
#include <DICOM Reader/Convert/Geometry/MarchingCubes.h>

using namespace std;
namespace fs = std::filesystem;
using namespace vxe;
using namespace DirectX;

using namespace concurrency;
using namespace vxe::med;
using namespace vxe::geo;

DICOMLoader::DICOMLoader(wstring dirname) :
	_installfolderpath(Windows::ApplicationModel::Package::Current->InstalledLocation->Path->Data()),
	_localfolderpath(Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data()),
	_cache(_localfolderpath / "DICOMVolumeDB")
{ 
	_cache.ReadCache();
	_MainFileSet = DICOMReader(_installfolderpath / dirname).MainFileSet;
	GatherVolumes();
	InitializeVolumes();
	LoadVolumes();
}

wstring DICOMLoader::GenerateDefaultFilename(wstring filetype) {
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	wstring timestr; timestr.resize(80);
	wcsftime(timestr.data(), 80, L"_%d%m%y_%H%M%S", &now);

	std::filesystem::path filepath(_cache.GetCachePath().parent_path());
	filepath /= filetype + timestr;
	return filepath;
}

shared_ptr<Mesh<VertexPosition, uint16_t>>
DICOMLoader::LoadPointCloud(Matter matter, vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {
	auto device = vanitycore->GetD3DDevice();

	shared_ptr<Mesh<VertexPosition, uint16_t>> pointmesh;
	pointmesh = make_shared<PointCloud2<VertexPosition, uint16_t>>();

	wstring FORUID = _utfconverter.from_bytes(volumeset[0].FORUID);
	wstring type = MatterName[matter].name + L"PointCloud";

	if (_caching) {
		fs::path meshFilename(_cache.Query(FORUID, type));
		if (fs::exists(meshFilename)) {
			tasks.push_back(pointmesh->LoadAsync(device, meshFilename));
			return pointmesh;
		}
	}

	std::vector<vert3> points = volumeset[0].GenerateIsoPointCloud(CorticalBone);
	char* data = (char*)points.data();
	vector<char> deserialized(data, data + points.size()*sizeof(vert3));
	tasks.push_back(pointmesh->CreateAsync(device, deserialized));

	if (_caching) {
		auto filepath = GenerateDefaultFilename(type);
		WriteVanityVertex<VertexPosition>((VertexPosition*)points.data(), nullptr, points.size(), 0, filepath);
		_cache.AddNewEntry({ FORUID, type, filepath.c_str()});
	}

	return pointmesh;
}

shared_ptr<Mesh<VertexPosition, uint16_t>>
DICOMLoader::LoadWireframeMesh(Matter matter, vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {
	auto device = vanitycore->GetD3DDevice();

	shared_ptr<Mesh<VertexPosition, uint16_t>> trimesh;
	trimesh = make_shared<Wireframe<VertexPosition, uint16_t>>();

	wstring FORUID = _utfconverter.from_bytes(volumeset[0].FORUID);
	wstring type = MatterName[matter].name + L"WireframeMesh";

	if (_caching) {
		fs::path meshFilename(_cache.Query(FORUID, type));
		if (fs::exists(meshFilename)) {
			tasks.push_back(trimesh->LoadAsync(device, meshFilename));
			return trimesh;
		}
	}

	std::vector<tri> Surface = MarchingCubes(volumeset[0].GetSamples(), HounsfieldScale[CorticalBone].lower);
	char* data = (char*)Surface.data();
	vector<char> deserialized(data, data + Surface.size()*sizeof(tri));
	tasks.push_back(trimesh->CreateAsync(device, deserialized));

	if (_caching) {
		auto filepath = GenerateDefaultFilename(type);
		WriteVanityVertex<VertexPosition>((VertexPosition*)Surface.data(), nullptr, Surface.size()*3, 0, filepath);
		_cache.AddNewEntry({ FORUID, type, filepath.c_str()});
	}

	return trimesh;
}

SceneTexture<Texture3D> DICOMLoader::LoadTexture3D(Matter matter, vector<task<void>>& tasks, shared_ptr<VanityCore>& vanitycore) {
	auto device = vanitycore->GetD3DDevice();

	uint32_t depth = static_cast<uint32_t>(volumeset[0].GetSamples().Depth());
	uint32_t width = static_cast<uint32_t>(volumeset[0].GetSamples().Width());
	uint32_t height = static_cast<uint32_t>(volumeset[0].GetSamples().Height());
	shared_ptr<Texture3D> texture3D = make_shared<Texture3D>(device, DXGI_FORMAT_R8_UNORM, width, height, depth, 1,
											D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);

	SceneTexture<Texture3D> scenetexture3D(texture3D);

	wstring FORUID = _utfconverter.from_bytes(volumeset[0].FORUID);
	wstring type = MatterName[matter].name + L"Texture3D";

	if (_caching) {
		fs::path textureFilename(_cache.Query(FORUID, type));
		if (fs::exists(textureFilename)) {
			scenetexture3D.Load(tasks, textureFilename);
			return scenetexture3D;
		}
	}

	auto VolumeTexture = volumeset[0].GenerateIsoSamples(CorticalBone);

	if (_caching) {
		auto filepath = GenerateDefaultFilename(type);
		WriteTexture3D(VolumeTexture, filepath);
		_cache.AddNewEntry({ FORUID, type, filepath.c_str()});
	}

	uint8_t* texturedata = VolumeTexture.Points();
	scenetexture3D.Load(tasks, vector<uint8_t>(texturedata, texturedata + depth * width * height));

	return scenetexture3D;
}
